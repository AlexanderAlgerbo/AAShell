#include "loop.h"
#include <windows.h>
#include <stdio.h>
void Loop(){
    // We want to read a line from stdin
    // Then parse it and execute it. our execute method will determines if we will continue reading. By doing so we could input a line like exit 
    char *line;
    char **args;
    int status;

    // Might place these in own methods like simulateRaw() and setDefaultState()
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
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
    rawMode &= ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT);
    rawMode |= ENABLE_PROCESSED_INPUT;
    SetConsoleMode(hStdin, rawMode);
    do{
        fputs("AA>",stdout);
        line = ReadLine(hStdin);
        // Arguments may change for this as i have not yet coded these methods.
        args = ParseLine(line);
        status = ExecuteLine(args);
        putchar('\n');
    }while(status);

    // Returns the mode to original as we exit the shell. Unsure if this will fire if the shell crashes. I do not know if c has try catch so we'll have to see how i might handle that
    SetConsoleMode(hStdin,originalMode);
    // This works because any non-zero value is the same as the boolean true in 
}


char* ReadLine(HANDLE hStdin){
    int buffSize = 1024;
    int pos = 0;
    //We allocate 1024 chars in the memory with initial value '\0' 
    char *buffer = calloc(buffSize,sizeof(char));

    // Will probably have to watch or read some quick documentation just so i understand these better later on.
    INPUT_RECORD record;
    DWORD read;
    // Infinite loop as we always want to read input for as long as the shell is active. Only end of line EOF or /n, ctrl +c will end the loop.
    while(1){
        ReadConsoleInput(hStdin,&record,1,&read);
        // Now i should be able to handle record to detect inputs. So it would be various if statements for various keypresses.
        // We begin with simple key presses and add more functionalty as we go after we get this to work.
        if(record.EventType == KEY_EVENT && record.Event.KeyEvent.bKeyDown){
            // If a key cannot be represented by ascci we need to use a virtual key code. Backspace needs this for example. fputs("\b \b", stdout);  // erase on screen
            char c = record.Event.KeyEvent.uChar.AsciiChar;
            WORD vk = record.Event.KeyEvent.wVirtualKeyCode;


            switch (vk)
            {
            case VK_RETURN:
                putchar('\n');
                //buffer[pos] = '\0';
                return buffer;
                break;
            case VK_BACK:
                if(pos > 0){
                    pos--;

                    for (int i = pos; buffer[i] != '\0'; i++)
                    {
                        // If we have values to the right of our current index we shift them left by one step
                        buffer[i] = buffer[i+1];
                    }

                    ShiftTerminalStringLeft(buffer,pos);
                    //fputs("\b \b",stdout);
                }
                break;
            case VK_LEFT:
                pos--;
                putchar('\b');
                // Everything that is to the right needs to move one step to the left.
                break;
            case VK_RIGHT:
                
                break;
            case VK_UP:
                
                break;
            case VK_DOWN:
                
                break;
            default:
                if(c != 0){
                    // Here we echo the char to terminal
                    putchar(c);
                    //printf("this is the key read: %c\n", c);
                    buffer[pos] = c;
                    pos++;
                }
                break;
            }
        }

        
        // Check buffer size, if exceeded reallocate more memory.
        if(pos >= buffSize){
            buffSize *= 2;
            buffer = realloc(buffer,buffSize);
            // Maybe check that it succeded and that buffer is not null. Because then we need to end the program.
        }
        //echoing to terminal so i can begin implementing the backspace handling.
        
    }

}
char **ParseLine(char *line){
    printf(line);
    return NULL;
}

int ExecuteLine(char **args){
    return 1;
}

void ShiftTerminalStringLeft(char *line, int pos){
    // Here we begin with fixing the line
    // WE use & as line[pos] gives us a value
    putchar('\b');
    // So either i get out of bounds or 
    fputs(&line[pos],stdout);
    // To "erase the last character"
    putchar(' ');
    int length = strlen(line);
    for (int i = pos; i <= length; i++)
    {
        putchar('\b');
    }
    
}

// Add arrow support next.

