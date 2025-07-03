record.Event.KeyEvent.bKeyDown is of type BOOL which is a number(usually 1) if keypress is down and 0 otherwise so in c you can treat it as a boolean in your if statements
as 0 is equivalent to false and all other numbers are true. This is basically a bool but it predates it as c did not have a built in bool type at the time.

For real time character by character input one will put the terminal in raw mode on unix type machines and simulate it on windows systems. 

Malloc(sizeof(char)* n) allocates n chars in memory.

When using ReadConsoleInput(hInput, &inputRecord, 1, &eventsRead);
it is fine to send a single reference to one inputrecord as long i say that we only want to read 1 input.
Same this for eventsRead. If we read more we need it to be a

in my for loop my condition was != '0' but what i wanted was '\0' and i had corrupted memory in my "string" because of it which made character shifting in terminal go wrong

Ansi escape codes to control cursor movement in terminal
1. Cursor Movement
Command	Effect	Example
\033[nA	    Move cursor up n lines	        \033[2A
\033[nB	    Move cursor down n lines	    \033[3B
\033[nC	    Move cursor right n columns	    \033[5C
\033[nD 	Move cursor left n columns  	\033[1D
\033[H or \033[row;colH	    Move to (row, col)	    \033[10;20H

This is preferable because it tracks the state of cursor position. It can therefore allow movement of cursor position without removing any of the visible chars. Which would
happen if you use "'\b' ' ' '\b'"