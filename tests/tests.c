#include <stdio.h>
#include <stdlib.h>
#include "../src/loop.h"

int assert_int_Eq(int expected, int actual)
{
    if (expected != actual)
    {
        fprintf(stderr, "Expected: %d. Actual: %d", expected, actual);
        return 1;
    }
    return 0;
}

int assert_Char_Eq(char expected, char actual)
{
    if (expected != actual)
    {
        fprintf(stderr, "Expected: %c. Actual: %c", expected, actual);
        return 1;
    }

    return 0;
}

int assert_CharArray_Eq(char *expected, char *actual)
{
    int length = strlen(expected);
    for (int i = 0; i < length; i++)
    {
        // if one char not equal we return immedietely
        if (assert_Char_Eq(expected[i], actual[i]) != 0)
        {
            return 1;
        }
    }

    return 0;
}

int test_LengthOfArrayOfCharPtr()
{
    // We initialize an array of char Ptr of known length. Then test that lengt
    int length = 5;
    int success = 0;
    char **lines = calloc(length, sizeof(char *));
    // Testing that it is initialized as expected
    success = assert_int_Eq(countWords(lines), 0);
    lines[0] = "Hello";
    lines[1] = "Yolo";
    // Testing that we detect 2 words
    success = assert_int_Eq(countWords(lines), 2);
    lines[2] = "Hello";
    // Testing that two equal words are counted seperately
    success = assert_int_Eq(countWords(lines), 3);
    lines[1] = NULL;
    // Testing that if we place a NULL before a word the word is not counted.
    success = assert_int_Eq(countWords(lines), 1);
    // Might be relevant to change parts of lines and

    if (success)
    {
        printf("Length of array of words test succeeded");
        return 1;
    }
    printf("Size of Array Test failed");
    return 0;
}
void test_CharPtrCopy()
{
    // Create an array, copy it, and compare the two. We also need to make sure the ptr are not equal
    char *array = "hello";
    char *copy = calloc(10, sizeof(char));
    if (!copy)
    {
        printf("Memory allocation failed.\n");
        return;
    }
    copy[0] = 'y';

    if (assert_CharArray_Eq(array, copy) == 0)
    {
        printf("Copy test failed. Somehow the strings are equal before copying.");
        free(copy);
        return;
    }
    stringCopy(copy, array);
    if (assert_CharArray_Eq(array, copy) != 0)
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
    printf("Copying test succeeded \n");
    free(copy);
    return;
}

void test_Combine_Of_Args()
{
    char **args = calloc(3, sizeof(char *));
    if (!args)
    {
        fprintf(stderr, "Failed to allocate memory");
    }
    // So we allocate the arg pointers. Then we allocate the char *. We can do this because our recombine method utilized strcat which copies the string/char* and appends it to the end of a line
    args[0] = "Hello";
    args[1] = "World";
    args[2] = "Working";
    if (strcmp(args[0], "Hello") != 0 || strcmp(args[1], "World") != 0 || strcmp(args[2], "Working") != 0)
    {
        fprintf(stderr, "The initialization of the different args have not worked correctly");
    }
    char *line = combineArgs(args);

    if (assert_CharArray_Eq(line, "Hello World Working") != 0)
    {
        fprintf(stderr, "The combination of args did not work. The combineArgs(args) did not return the expected result");
        return;
    }

    printf("The recombination of char ** args works as intended");
}

int main()
{
    test_CharPtrCopy();
    printf("Arrived here");
    test_LengthOfArrayOfCharPtr();
    test_Combine_Of_Args();
}