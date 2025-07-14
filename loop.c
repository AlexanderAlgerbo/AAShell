#include "loop.h"
#include <windows.h>
#include <stdio.h>
void loop(){

    char *line;
    char **args;
    int status;

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
    // this should allocate 64 pointers to strings which i then can allocate in readLine.
    int amountOfLines = 64;
    char **lines = calloc(amountOfLines,sizeof(char*));
    DWORD originalMode;
    // Get console mode sets current mode in originalMode
    GetConsoleMode(hStdin, &originalMode);
    DWORD rawMode = originalMode;
    rawMode &= ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT);
    rawMode |= ENABLE_PROCESSED_INPUT;
    SetConsoleMode(hStdin, rawMode);
    do{
        fputs("AA>",stdout);
        line = readLine(hStdin,lines);
        // Arguments may change for this as i have not yet coded these methods.
        args = parseLine(line);
        status = executeLine(args);
        putchar('\n');
    }while(status);
    SetConsoleMode(hStdin,originalMode);
    // This works because any non-zero value is the same as the boolean true in 
}


char* readLine(HANDLE hStdin,char **lines){
    int buffSize = 1024;
    int pos = 0;
    int clPos = 0;// The command line position. 
    //We allocate 1024 chars in the memory with initial value '\0' 
    char *buffer = calloc(buffSize,sizeof(char));
    char *tempLine = calloc(buffSize,sizeof(char));


    // Will probably have to watch or read some quick documentation just so i understand these better later on.
    INPUT_RECORD record;
    DWORD read;
    // Infinite loop as we always want to read input for as long as the shell is active. Only end of line EOF or /n, ctrl +c will end the loop.
    while(1){
        ReadConsoleInput(hStdin,&record,1,&read);
        if(record.EventType == KEY_EVENT && record.Event.KeyEvent.bKeyDown){
            char c = record.Event.KeyEvent.uChar.AsciiChar;
            WORD vk = record.Event.KeyEvent.wVirtualKeyCode;

            lines[0] = buffer;
            switch (vk)
            {
            case VK_RETURN:
                putchar('\n');
                for (int i = lengthOfArray(lines)-1; i >= 0; i--)
                {
                    
                    lines[i+1] = lines[i];
                }
                return buffer;
                break;
            case VK_BACK:
                if(pos > 0){
                    pos--;
                    shiftTerminalStringLeft(buffer,pos);
                }
                break;
            case VK_LEFT:
                if(pos > 0){
                    pos--;
                    printf("\033[D");
                }
                break;
            case VK_RIGHT:
                if(pos < strlen(buffer)){
                    pos++;
                    printf("\033[C");
                }
                break;
            case VK_UP:
                if(clPos == 0){
                    stringCopy(tempLine,lines[clPos]);
                }
                if(clPos <63 && lines[clPos + 1] != NULL){
                    clPos++;
                    printf("\033[2K\r");
                    stringCopy(buffer,lines[clPos]);
                    pos = strlen(buffer);
                    fputs("AA>",stdout);
                    printf(buffer);

                }
                
                break;
            case VK_DOWN:
                
                if(clPos > 1){
                    clPos--;
                    printf("\033[2K\r");
                    fputs("AA>",stdout);
                    //buffer = lines[clPos];
                    stringCopy(buffer,lines[clPos]);
                    pos = strlen(buffer);
                    printf(buffer);
                }
                else if (clPos == 1){
                    clPos--;
                    printf("\033[2K\r");
                    fputs("AA>",stdout);

                    // It will never be null. 
                    if(lines[0] != NULL){
                        stringCopy(buffer,tempLine);
                        pos = strlen(buffer);
                        printf(buffer);
                    }
                }
                break;
            default:
                if(c != 0){
                    // I think the problem is around here and i believe it to be the buffer that is wrong
                    
                    putchar(c);
                    shiftTerminalStringRight(buffer,pos);
                    buffer[pos] = c;
                    pos++;
                }
                break;
            }
        }
 
        if(pos >= buffSize){
            buffSize *= 2;
            buffer = realloc(buffer,buffSize);
        }
        
    }

}

char **parseLine(char *line){
    // Might start simple an simply go through the line, whenever a blankspace is found everything from beginningPos to curPos will be copied to a new string.
    int startPos = 0;
    int pos = 0;
    // i use calloc because i want them to be initialized as null. The only annoying part is that there is no reCalloc so i will have to make ony myself.
    char **args = calloc(8,sizeof(char *));
    // Unsure of what a good size for each char pointer but each arg should not be uneccessarily large. Maybe 64 is a good start. As one may want to send file paths which can be long
    for (size_t i = 0; i < strlen(line); i++)
    {
        // Maybe a switch case is more ellegant here as it is a char which will always be a single value
        if(line[i] ==' '){
            //Split
        }
        else if(line[i] == '#'){
            // Stop, we will simply ignore the rest
        }

    }
    
    printf(line);
    return NULL;
}

int executeLine(char **args){
    return 1;
}

void shiftTerminalStringLeft(char *line, int pos){
    // Here we begin with fixing the line
    // WE use & as line[pos] gives us a value
    for (int i = pos; line[i] != '\0'; i++)
    {
        // If we have values to the right of our current index we shift them left by one step
        line[i] = line[i + 1];
    }

    printf("\033[D");
    fputs(&line[pos],stdout);
    putchar(' ');
    int length = strlen(line);
    int l = length - (pos+1);
    printf("\033[%dD",length - (pos-1));   
}

void shiftTerminalStringRight(char *line,int pos){
    int length = strlen(line);
    for (int i = length-1; i >= pos; i--)
    {
        
        line[i+1] = line[i];
    }
    
    fputs(&line[pos+1],stdout); 
    int l = length - (pos+1);
    printf("\033[%dD",length - (pos+1));

}

int lengthOfArray(char** array){
    int size = 0;
    while(array[size] != NULL){
        size++;
    }
    return size;
}

void stringCopy(char *buffer,char *src){
    int length = strlen(src) +1;// +1 because of null termination.
    memcpy(buffer,src,length);// Because i already have my buffer allocated in memory already this should co through and copy the memory of src to buffer. 
    // Should probably add a memory reallocation here as well because src could be larger than buffer if i previously wrote a line that surpassed 1024 chars.
    // It is becoming more and more relevant for me to write a reallocation code.
    // While (char != '\0') after length make it '\0'
    for (int i = length; buffer[i] != '\0'; i++)
    {
        buffer[i] = '\0';
        /* code */
    }
    
    
}


