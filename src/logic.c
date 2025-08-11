#include "logic.h"
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
int countWords(char **array)
{
    int size = 0;
    //
    while (array[size] != NULL)
    {
        size++;
    }
    return size;
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