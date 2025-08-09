#include "commands.h"
#include <unistd.h>
#include <limits.h>
#include <stdio.h>
#include <stddef.h>
#include <errno.h>
#include <string.h>
#include <direct.h>
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