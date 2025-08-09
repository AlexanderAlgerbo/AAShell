#include "loop.h"
#include <stdio.h>
#include <windows.h>
#include <ctype.h>
#include "commands.h"
#include "logic.h"

typedef void (*CommandHandler)(char **args);

typedef struct
{
  char *name;
  CommandHandler handler;
} Command;

void loop()
{

  char *line;
  char **args;
  int status;

  HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);

  int amountOfLines = 64;
  char **lines = calloc(amountOfLines, sizeof(char *));
  DWORD originalMode;
  // Get console mode sets current mode in originalMode
  GetConsoleMode(hStdin, &originalMode);
  DWORD rawMode = originalMode;
  rawMode &= ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT);
  rawMode |= ENABLE_PROCESSED_INPUT;
  SetConsoleMode(hStdin, rawMode);
  do
  {
    fputs("AA>", stdout);
    line = readLine(hStdin, lines);
    args = parseLine(line);
    status = executeLine(args);
    putchar('\n');
  } while (status);
  SetConsoleMode(hStdin, originalMode);
}

char *readLine(HANDLE hStdin, char **lines)
{
  int buffSize = 1024;
  int pos = 0;
  int clPos = 0;
  char *buffer = calloc(buffSize, sizeof(char));
  char *tempLine = calloc(buffSize, sizeof(char));

  INPUT_RECORD record;
  DWORD read;
  while (1)
  {
    ReadConsoleInput(hStdin, &record, 1, &read);
    if (record.EventType == KEY_EVENT && record.Event.KeyEvent.bKeyDown)
    {
      char c = record.Event.KeyEvent.uChar.AsciiChar;
      WORD vk = record.Event.KeyEvent.wVirtualKeyCode;

      lines[0] = buffer;
      switch (vk)
      {
      case VK_RETURN:
        putchar('\n');
        for (int i = countWords(lines) - 1; i >= 0; i--)
        {

          lines[i + 1] = lines[i];
        }
        return buffer;
        break;
      case VK_BACK:
        if (pos > 0)
        {
          pos--;
          shiftTerminalStringLeft(buffer, pos);
        }
        break;
      case VK_LEFT:
        if (pos > 0)
        {
          pos--;
          printf("\033[D");
        }
        break;
      case VK_RIGHT:
        if (pos < strlen(buffer))
        {
          pos++;
          printf("\033[C");
        }
        break;
      case VK_UP:
        if (clPos == 0)
        {
          stringCopy(tempLine, lines[clPos]);
        }
        if (clPos < 63 && lines[clPos + 1] != NULL)
        {
          clPos++;
          printf("\033[2K\r");
          stringCopy(buffer, lines[clPos]);
          pos = strlen(buffer);
          fputs("AA>", stdout);
          printf(buffer);
        }

        break;
      case VK_DOWN:

        if (clPos > 1)
        {
          clPos--;
          printf("\033[2K\r");
          fputs("AA>", stdout);
          stringCopy(buffer, lines[clPos]);
          pos = strlen(buffer);
          printf(buffer);
        }
        else if (clPos == 1)
        {
          clPos--;
          printf("\033[2K\r");
          fputs("AA>", stdout);
          if (lines[0] != NULL)
          {
            stringCopy(buffer, tempLine);
            pos = strlen(buffer);
            printf(buffer);
          }
        }
        break;
      default:
        if (c != 0)
        {

          putchar(c);
          shiftTerminalStringRight(buffer, pos);
          buffer[pos] = c;
          pos++;
          fflush(stdout);
        }
        break;
      }
    }

    if (pos >= buffSize)
    {
      buffSize *= 2;
      buffer = realloc(buffer, buffSize);
    }
  }
}

char **parseLine(char *line)
{
  int startPos = -1; // if -1 we are in whitespace territory
  int pos = 0;
  int nbrArgs = 0;
  char **args = calloc(8, sizeof(char *));
  int comment = 1;
  for (size_t i = 0; i < strlen(line) && comment; i++)
  {
    pos = i;
    switch (line[i])
    {
    case ' ':
      if (startPos != pos && startPos != -1)
      {
        int length = pos - startPos;
        args[nbrArgs] = calloc(length + 1, sizeof(char));
        memcpy(args[nbrArgs], line + startPos, length);
        startPos = -1;
        nbrArgs++;
      }
      break;
    case '#':
      pos--;
      comment = 0;
      break;
    default:
      if (startPos == -1)
      {
        startPos = i;
      }
      break;
    }
  }

  if (startPos != -1)
  {
    int length = pos + 1 - startPos;
    args[nbrArgs] = calloc(length + 1, sizeof(char));
    memcpy(args[nbrArgs], line + startPos, length);
    startPos = -1;
    nbrArgs++;
  }
  /*
  printf("%d", nbrArgs);
  for (size_t i = 0; i < nbrArgs; i++)
  {
    printf(args[i]);
  }
  */
  return args;
}

int executeLine(char **args)
{
  char *temp = calloc(strlen(args[0]), sizeof(char));
  stringCopy(temp, args[0]);
  for (size_t i = 0; i < strlen(temp); i++)
  {
    temp[i] = tolower(temp[i]);
  }

  Command commands[] = {
      {"pwd", handlePWD}, {"cd", handleCD}, {NULL, NULL}};

  for (size_t i = 0; commands[i].name != NULL; i++)
  {
    if (strcmp(commands[i].name, temp) == 0)
    {

      commands[i].handler(args);
      // May want My handle functions to return an int so that i can judge if they were successful or not. But as i print the failure in the functions either way it probably does not matter
      printf("\n");
      return 1;
    }
  }

  char *cmd = combineArgs(args);
  PROCESS_INFORMATION procINFO;
  STARTUPINFO startInfo = {sizeof(startInfo)};
  if (CreateProcess(NULL, cmd, NULL, NULL, FALSE, 0, NULL, NULL, &startInfo, &procINFO))
  {
    printf("CreateProcess worked\n");
    WaitForSingleObject(procINFO.hProcess, INFINITE);
    printf("We passed the wait for object\n");
    // Could get an exit code with DWORD exit and then GetExitCode(PROCESS Handle, &exit);

    CloseHandle(procINFO.hProcess);
    CloseHandle(procINFO.hThread);
  }
  else
  {
    DWORD err = GetLastError();
    printf("Failed to launch, Error: %d", err);
    return 0;
  }

  return 1;
}

void shiftTerminalStringLeft(char *line, int pos)
{
  for (int i = pos; line[i] != '\0'; i++)
  {
    line[i] = line[i + 1];
  }

  printf("\033[D");
  fputs(&line[pos], stdout);
  putchar(' ');
  int length = strlen(line);
  printf("\033[%dD", length - (pos - 1));
}

void shiftTerminalStringRight(char *line, int pos)
{
  int length = strlen(line);
  for (int i = length - 1; i >= pos; i--)
  {

    line[i + 1] = line[i];
  }

  fputs(&line[pos + 1], stdout);

  int tailLength = strlen(&line[pos + 1]) - 1; // subtract inserted char
  if (tailLength > 0)
    printf("\033[%dD", tailLength + 1);
}

void stringCopy(char *buffer, char *src)
{
  int length = strlen(src) + 1; // +1 because of null termination.
  memcpy(buffer, src, length);
  for (int i = length; buffer[i] != '\0'; i++)
  {
    buffer[i] = '\0';
    /* code */
  }
}

char *combineArgs(char **args)
{
  // I calculate size to allocate
  size_t length = 0;
  size_t argCount = countWords(args);
  // So countWords is wrong. It returns 6 here for some reason
  for (size_t i = 0; i < argCount; i++)
  {
    /* code */
    length += strlen(args[i]);
    if (i < argCount - 1)
    {
      length += 1;
    }
  }
  length += 1; // For a null terminator

  // Allocate the line. As i will fill it with immediately it may be better to use malloc but cba at the moment
  char *line = calloc(length, sizeof(char));
  if (!line)
  {
  }
  // Recombine it
  for (size_t i = 0; i < argCount; i++)
  {
    // We go through all arguments. Place them into the cmd line
    strcat(line, args[i]);
    // places a blankspace until we are at the last arg
    if (i < argCount - 1)
    {
      strcat(line, " ");
    }
  }

  return line;
}
