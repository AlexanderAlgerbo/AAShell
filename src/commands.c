#include "commands.h"
#include <unistd.h>
#include <limits.h>
#include <stdio.h>
#include <stddef.h>
#include <errno.h>
#include <string.h>
#include <direct.h>
#include <windows.h>
#include <sys/stat.h>
#include "logic.h"
void handlePWD(char **args)
{
    if (countWords(args) != 1)
    {
        printf("Wrong number of args");
        return;
    }

    char wd[PATH_MAX];
    if (getcwd(wd, sizeof(wd)) != NULL)
    {
        printf(wd);
    }
    else
    {
        fprintf(stderr, "getcwd failed");
    }
}

void handleCD(char **args)
{
    // Check lenght of args should be 2. We take args[1] as path.
    // We will want to take the arg and put the whole path before it.
    // do if(chDir(args[1]) != 0)
    if (_chdir(args[1]) != 0)
    {

        // errno is thread local and methods like chdir write to that variable with an error code. strerror takes that error code and prints it in words.
        fprintf(stderr, "Failed to change directory to %s, %s \n", args[1], strerror(errno));
        return;
    }

    // Will not print what i changed to because it will be displayed already if successfull
}

void handleEcho(char **args)
{
    char *buffer = calloc(100, sizeof(char));
    if (countWords(args) < 2)
    {
        buffer[0] = '\0';
    }
    else
    {
        buffer = combineArgs(&args[1]);
    }

    printf("%s", buffer);
}

void handleLS(char **args)
{
    // Should also let one send a file path that we can place in findfirstFile.
    // We need to send with windows pathing. So in this case C:\Users\alexa\Documents\GitHub\AAShell
    int temp = countWords(args);
    if (temp > 2)
    {
        fprintf(stderr, "Too many arguments sent to command ls. Expected 0 or 1, Actual: %d", countWords(args));
    }

    // appends * to args[1] We should always be able to use arg 1 because we should have initialized it using calloc. Otherwise i may just do that. Or allocate memory for it here if it does not exits.
    char *buffer = appendWildCard(args[1]);
    WIN32_FIND_DATA findData;
    printf("%s \n", buffer);
    HANDLE fileHandle = FindFirstFile(buffer, &findData);

    if (fileHandle == INVALID_HANDLE_VALUE)
    {
        fprintf(stderr, " Could not establish a handle to search for files");
        return;
    }

    do
    {
        if (strcmp(findData.cFileName, ".") != 0 && strcmp(findData.cFileName, "..") != 0 && !(FILE_ATTRIBUTE_HIDDEN & findData.dwFileAttributes))
        {
            printf("%s ", findData.cFileName);
        }
    } while (FindNextFile(fileHandle, &findData) != NULL);
}

char *appendWildCard(char *path)
{
    if (!path)
    {
        char *wildcard = malloc(2);
        if (wildcard)
            strcpy(wildcard, "*");
        return wildcard;
    }
    size_t len = strlen(path);
    char *buffer = malloc(len + 3); // Lengt of path and \\*
    if (!buffer)
    {
        return NULL;
    }

    strcpy(buffer, path);
    if (len == 0 || (buffer[len - 1] != '\\' & buffer[len - 1] != '/'))
    {
        buffer[len] = '\\';
        buffer[len + 1] = '*';
        buffer[len + 2] = '\0';
    }
    else
    {
        buffer[len + 1] = '*';
        buffer[len + 2] = '\0';
    }

    return buffer;
}

void handleClear(char **args)
{
    // we simply ignore args as we will always clear everything.
    HANDLE hConsole;
    COORD coordScreen = {0, 0};
    DWORD cCharsWritten;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD dwConSize;

    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hConsole == INVALID_HANDLE_VALUE)
    {
        fprintf(stderr, "Could not establish a handle to standart input");
    }
    printf("Clearing");
    if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
    {
        return;
    }

    dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

    if (!FillConsoleOutputCharacter(hConsole, (TCHAR)' ', dwConSize, coordScreen, &cCharsWritten))
    {
        return;
    }

    if (!FillConsoleOutputAttribute(hConsole, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten))
    {
        return;
    }

    SetConsoleCursorPosition(hConsole, coordScreen);
}

void handleMakeDir(char **args)
{
    if (countWords(args) != 2)
    {
        fprintf(stderr, "Wrong amount of args");
        return;
    }

    if (!_mkdir(args[1]))
    {
        printf("Created directory %s", args[1]);
    }
    else
    {
        fprintf(stderr, "Failed to create directory");
    }
}

void handleRMDir(char **args)
{
    if (countWords(args) != 2)
    {
        fprintf(stderr, "Wrong amount of args");
        return;
    }

    if (!_rmdir(args[1]))
    {
        printf("Removed directory %s", args[1]);
    }
    else
    {
        fprintf(stderr, "Failed to remove directory");
        if (errno == ENOENT)
        {
            fprintf(stderr, ": No entry of that name");
        }
    }
}

void handleShellExec(char **args)
{
    // In both open and find we check if arguments are as expected and then give runShellExec the args.
    char *arguments = NULL;
    // I think this is the only case i need to handle. As shellExecuteEx should fail if the file does not exist or parameters are wrong.
    size_t len = countWords(args);
    if (len < 2)
    {
        fprintf(stderr, "No file was sent to open");
        return;
    }
    if (len > 2)
    {
        arguments = combineArgs(&args[2]); // Combines all parameters for shellExecute.
    }

    runShellExec(args[0], args[1], arguments);
}

void runShellExec(char *verb, char *file, char *args)
{
    SHELLEXECUTEINFO shExInfo = {0};
    shExInfo.cbSize = sizeof(SHELLEXECUTEINFO);
    shExInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
    shExInfo.hwnd = NULL;
    shExInfo.lpVerb = verb;       // the action
    shExInfo.lpFile = file;       // file or program
    shExInfo.lpParameters = args; // optional arguments
    shExInfo.lpDirectory = NULL;
    shExInfo.nShow = SW_SHOWNORMAL;
    shExInfo.hInstApp = NULL;

    if (ShellExecuteEx(&shExInfo))
    {
        WaitForSingleObject(shExInfo.hProcess, INFINITE);
        CloseHandle(shExInfo.hProcess);
    }
    else
    {
        DWORD err = GetLastError();
        LPSTR msgBuf = NULL;

        FormatMessageA(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL, err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&msgBuf, 0, NULL);

        printf("Error %lu: %s\n", err, msgBuf ? msgBuf : "Unknown error");
        LocalFree(msgBuf);
    }
}

void handleExit(char **args)
{
    exit(0);
}

// So it does not work as intended at the moment