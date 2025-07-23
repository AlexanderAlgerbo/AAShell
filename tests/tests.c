#include <stdio.h>
#include <stdlib.h>
#include "../src/loop.h"

int assert_int_Eq(int expected, int actual)
{
    if (expected != actual)
    {
        fprintf(stderr, "Expected: %d. Actual: %d", expected, actual);
        return 0;
    }
    return 1;
}

int assert_Char_Eq(char expected, char actual)
{
    if (expected != actual)
    {
        fprintf(stderr, "Expected: %c. Actual: %c", expected, actual);
        return 0;
    }

    return 1;
}

int assert_CharArray_Eq(char *expected, char *actual)
{
    int length = strlen(expected);
    for (int i = 0; i < length; i++)
    {
        // if one char not equal we return immedietely
        if (assert_Char_Eq(expected[i], actual[i]) != 0)
        {
            return 0;
        }
    }

    return 1;
}

int test_LengthOfArrayOfCharPtr()
{
    // We initialize an array of char Ptr of known length. Then test that lengt
    char **lines = calloc(5, sizeof(char *));
}

void test_CharPtrCopy()
{
    // Create an array, copy it, and compare the two. We also need to make sure the ptr are not equal
    int success = 0;
    char *array = "hello";
    char *copy = calloc(10, sizeof(char));
    if (!copy)
    {
        printf("Memory allocation failed.\n");
        return;
    }
    copy[0] = 'y';

    if (assert_CharArray_Eq(array, copy) == 1)
    {
        printf("Copy test failed. Somehow the strings are equal before copying.");
        free(copy);
        return;
    }
    stringCopy(copy, array);
    if (assert_CharArray_Eq(array, copy) == 0)
    {
        printf("Copying failed. Char array not equals.");
        free(copy);
        return;
    }
    if (copy == array)
    {
        printf("Points to the same memory space. Test failed as it is now not a copy");
        free(copy);

        return;
    }
    printf("Copying test succeeded");
    free(copy);
    return;
}

int main()
{
    test_CharPtrCopy();
}