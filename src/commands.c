#include "commands.h"
#include <unistd.h>
#include <limits.h>
#include <stdio.h>
#include <stddef.h>
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