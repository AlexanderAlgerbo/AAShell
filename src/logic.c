#include "logic.h"
#include <stdio.h>
#include <stddef.h>
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