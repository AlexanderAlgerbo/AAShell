#include "commands.h"
#include <unistd.h>
#include <limits.h>
#include <stdio.h>
#include <stddef.h>
#include <errno.h>
#include <string.h>
#include <direct.h>
#include <windows.h>
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

// So it does not work as intended at the moment