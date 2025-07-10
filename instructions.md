
A shell does three main things. Initialization from various configuration files, Interpreting commands and exectuting them and at last terminating and freeing up memory

The shell will basically initialize, run a loop of commands, this will in my case be from stdin as i want an interactive shell but could also be from a file. and then terminate

To begin with i will utilize a simple initialization without any specific configuration. The loop itself is built on three attributes. Reading the input, Parsing the input, and
then executing the command with given variables. Have installed c compiler and am ready to begin coding tomorrow.

For future additions i may use readline library. But to begin with we will attempt to make a shell for windows and in the future see about making it compatible with unix OS as well
So an option in unix systems to get real time character input seems to be to put the shell in raw mode.


Time to test reading a line now an see if we get real time character by character .

To Do Next Time: implement right,up and down arrow keys and then we can be satisfied for now on reading a line and move on to parsing it into program and arguments. 

Should probably make tests for the various functions if i ever want to add on more functionality later.

Bugtest arrow key up and down. Right now printing needs to be fixed a bit, and AAShell needs to be printed as well. 
. Maybe it is something about where i define my clPos.

Current bug that makes some lines get overwritten and it works a bit strange. I think it is because i use buffer = lines[clPos] and then when we edit buffer we edit that line
yet that is not what we want to do. What should be done is that we always edit lines[0] so we will need to retrieve a copy of lines. So i will probably have to write a copy function if it does not already exist.


Important to add. As i have dynamically allocated arrays i will need no free every line in lines by looping over free(lines[i]). and then free(lines)
I will also need to make a recalloc function becuase of how i have implemented my project. But as it is very rare that you would need to exceed 1024 characters to write the program
you would wish to run i will have that as an extra thing to add after i have gotten everything into working order.

Copy works as intended. Now i need to look into down arrow key as that one print the wrong thing if i have pressed down arrow multiple times when im at pos 1 or 0. It tends to print ed or two chars which i cannot remove so i would assume my pos is not carried over. At the same time if i were to have something in buffer it is not carried over if i press arrow up once and then down once. Probably need to reset clPos as well