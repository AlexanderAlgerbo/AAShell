record.Event.KeyEvent.bKeyDown is of type BOOL which is a number(usually 1) if keypress is down and 0 otherwise so in c you can treat it as a boolean in your if statements
as 0 is equivalent to false and all other numbers are true. This is basically a bool but it predates it as c did not have a built in bool type at the time.

For real time character by character input one will put the terminal in raw mode on unix type machines and simulate it on windows systems. 

Malloc(sizeof(char)* n) allocates n chars in memory.
When allocating memory on example char ** stringArray = Malloc(sizeof(char*) * n) we do not allocate the memory of every char* this array will hold. We only allocate the memory of
the pointers to the "string", in our case char *array, will take.

When using ReadConsoleInput(hInput, &inputRecord, 1, &eventsRead);
it is fine to send a single reference to one inputrecord as long i say that we only want to read 1 input.
Same this for eventsRead. If we read more we need it to be a

in my for loop my condition was != '0' but what i wanted was '\0' and i had corrupted memory in my "string" because of it which made character shifting in terminal go wrong

Ansi escape codes to control cursor movement in terminal
1. Cursor Movement
Command	Effect	Example
\033[nA	    Move cursor up n lines	        \033[nA
\033[nB	    Move cursor down n lines	    \033[nB
\033[nC	    Move cursor right n columns	    \033[nC
\033[nD 	Move cursor left n columns  	\033[nD
\033[H or \033[row;colH	    Move to (row, col)	    \033[10;20H

clear line is done by \033[2K

This is preferable because it tracks the state of cursor position. It can therefore allow movement of cursor position without removing any of the visible chars when undoing a '\b'

At the moment am unsure of how global variables would work for a file. Created one but when compiling it did not seem to work properly. 

Using printf on NULL variable as string will crash but if we pass it by a %p argument it will not crash as we pass the pointer then and it will print something like (null).

when clearing a line it is important to return cursor to beginning otherwise printf and such will start at wrong place

If i want to "simulate" private functions to encapsulate a part of my code i can declare them static. Then they only show up as accessible in that header and source file.

Calloc is a decent way to initialize a "safe" string/buffer when you want to be able to use strlen and such while building it, as long as i make sure that it is properly null terminated in the end. Which i do when pressing enter. Also need to make sure that you cannot write to the last index before allocating more memory to it as that could potentially allow one to overwrite last char that needs to be null terminated. Otherwise methods like printf and strlen will continue reading the memory after your buffer has ended.

When allocating memory the size of the data and other meta data is stored in the beginnning of the memory space allocated. Say you were to allocate 10 chars. The space would be slightly larger to accomodate this metadata.

When freeing memory it is important to also turn the pointer null so that you do not continue pointing to memory that is freed as that causes undefined behaviour.

Switch case utilizes a jump table for simple cases and binary search for more complicated. Respectively O(1) and O(log(n)) time complexity. While if statements go through every case until it finds the one it is looking for. So O(n). 

For my parser i could at the moment simply use strtok to tokenize it based on delimeters such as ' ' and '#'. But to enable easy expansion for things like quotation handling and others i make my own parsing function.

Unsure of why MSYS is slower at writing outputs but it could be the emulation of a posix environment that creates overhead that slows it down.

For parsing i could simply give pointers to the original line where i have allocated memory of a certain length. But by doing this i compromise the args if my line is ever changed and i cannot reliably change each argument without affecting the line. Copying is safer and will avoid unnecessary bugs.

when using execvp to execture args it is important to make sure args is null terminated and that every singular argument is null terminated as well. Otherwise it would continue to read the memory after our arguments/argument has ended.