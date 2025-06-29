#ifndef LOOP_H
#define LOOP_H
#include <windows.h>

// Am not sure exactly what functions i will need at the moment except for myLoop.
void Loop();
// The loop will need to read input, parse input and execute it. So we eill need a read, parse and execute function
// Will allocate memory go through the line and place them on the memory, if size is exceeded reallocate memory and 
// Will utilize low level posix commands as it could be good to handle commands like ctrl+c if i can be bothered to implement it
char *ReadLine(HANDLE hStdin);

// We also need a parseline/split line that divides our string into program and arguments
// basically an array of string i will allocate manually. The pointers will be stored in order. But for every string i add the memory position does not have to be in order of
// index. 
char **ParseLine(char *line);

// Either a void or int. If i return an int it can signify successfull completion and promt the reading of a new line
int ExecuteLine(char **args);

#endif


// Functions like fgets() reads from a memory until they hit '/0' character. 