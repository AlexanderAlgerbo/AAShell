#include "loop.h"
#include <windows.h>
void myLoop(){
    // We want to read a line from stdin
    // Then parse it and execute it. our execute method will determines if we will continue reading. By doing so we could input a line like exit 
    char *line;
    char **args;
    int status;

    // Might place these in own methods like simulateRaw() and setDefaultState()
    hStdin = GetStdHandle(STD_INPUT_HANDLE);
    /*
    I can utilize this to determine if it is interactive capabilites i need or if i simply want to read of a script
    // May turn readLine into two methods like interactive readLine and another for files. But files is an afterthought or an extra feature.
    // The focus is on the interactive shell part.
    DWORD type = GetFileType(hStdin);

    if (type == FILE_TYPE_CHAR) {
        // It's a terminal (console) — use ReadConsoleInput
    } else if (type == FILE_TYPE_DISK || type == FILE_TYPE_PIPE) {
        // It's a file or pipe — use ReadFile or fgets
    }
    */

    DWORD originalMode;
    // Get console mode sets current mode in originalMode
    GetConsoleMode(hStdin, &originalMode);
    DWORD rawMode = originalMode;
    rawMode &= ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT | ENABLE_PROCESSED_INPUT);
    SetConsoleMode(hStdin, rawMode);
    do{
        line = ReadLine();
        // Arguments may change for this as i have not yet coded these methods.
        args = ParseLine(line);
        status = ExecuteLine(args);
    }while(status);

    // Returns the mode to original as we exit the shell. Unsure if this will fire if the shell crashes. I do not know if c has try catch so we'll have to see how i might handle that
    SetConsoleMode(hStdin,originalMode);
    // This works because any non-zero value is the same as the boolean true in 
}

/*
// We will begin with only handling 1 event at a time.
Then we can use ReadConsoleInput() by 
INPUT_RECORD record;
DWORD read;
while (1) {
    ReadConsoleInput(hStdin, &record, 1, &read);
    // process record...
}
*/
char* ReadLine(){
    int buffSize = 1024;
    int pos = 0;
    // This malloc with buffSize * sizeof(char) <==> allocate memory of 1 byte * buffsize. So in our case 1024 chars are allocated. 
    char *buffer = malloc(buffSize *sizeof(char));

    // Will probably have to watch or read some quick documentation just so i understand these better later on.
    INPUT_RECORD record;
    DWORD read;
    // Infinite loop as we always want to read input for as long as the shell is active. Only end of line EOF or /n, ctrl +c will end the loop.
    while(1){
        ReadConsoleInput(hStdin,buffer,1,read);
        // Now i should be able to handle record to detect inputs. So it would be various if statements for various keypresses.
        // We begin with simple key presses and add more functionalty as we go after we get this to work.
        if(record.EventType == KEY_EVENT && record.Event.KeyEvent.bKeyDown){
            // If a key cannot be represented by ascci we need to use a virtual key code. Backspace needs this for example. fputs("\b \b", stdout);  // erase on screen
            char c = record.Event.KeyEvent.uChar.AsciiChar;
            
            if(c == '\r'){
                // Enter has been pressed and reading should stop. Any other button press should simply be placed in the buffer
                buffer[pos] = '\0';
                return buffer;
            }
            buffer[pos] = c;
            pos++;
        }

        
        // Check buffer size, if exceeded reallocate more memory.
        if(pos >= buffSize){
            buffSize *= 2;
            buffer = realloc(buffer,buffSize);
            // Maybe check that it succeded and that buffer is not null. Because then we need to end the program.
        }
        // If i allow backspace i wonder if the terminal will handle it automatically in "raw" mode or if i will have to send back the current "line"/buffer to the terminal

        
    }

}

