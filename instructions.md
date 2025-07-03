
A shell does three main things. Initialization from various configuration files, Interpreting commands and exectuting them and at last terminating and freeing up memory

The shell will basically initialize, run a loop of commands, this will in my case be from stdin as i want an interactive shell but could also be from a file. and then terminate

To begin with i will utilize a simple initialization without any specific configuration. The loop itself is built on three attributes. Reading the input, Parsing the input, and
then executing the command with given variables. Have installed c compiler and am ready to begin coding tomorrow.

For future additions i may use readline library. But to begin with we will attempt to make a shell for windows and in the future see about making it compatible with unix OS as well
So an option in unix systems to get real time character input seems to be to put the shell in raw mode.


Time to test reading a line now an see if we get real time character by character .

To Do Next Time: Switch to using ansi escape codes to move the cursor.implement right,up and down arrow keys and then we can be satisfied for now on reading a line and move on to parsing it into program and arguments. 