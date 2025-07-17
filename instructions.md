
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

So the only bug at the moment that i find is that when index is zero we do not clear the

The reason it is not cleared is probably that we have copied every line onto buffer which we have equal to lines[0]. So lines[0] would have become a former line
 In addition to that index 1 and 2 seems to be the former line

 SO up arrow key goes through all indexes and index 1 and 2 are as they should be. It seems like the problem is not that there are 2 multiples of last line but that it takes two presses of down arrow key to go down from index 1 to 0. Which is very strange. 
 Answer: Figured it out. IN my if statement i have clPos > 0 and then in the statement i do clPos--,

 So only bug left is that sometimes when returning and we have something in lines[0] or tempLine we print more the next time we press any character button.
 It looks like it somehow prints the previous line somehow. So somewhere we may accidentaly overwrite or but the previous line into a charbuffer that then gets printed when we press a button

 So something with & and getting the reference seems to fuck it all up. It seems like we return to what we had. And if it is smaller or less than the line former the memory space of that former line will not have been reset. So when we then print it out from pos +1 we get everything after. Might even be that we somehow just get a reference to the former line for some reason. First thought would be to reset the rest of the "string" or char pointer when  we copy to make sure that the memory gets cleaned up i guess. Another thought is if i can simply free up buffer and allocate a new one. But it might be simpler to go through the rest of the string and reset it.

 Time for parsing, and here i need to determine what functionality i want to achieve for my shell. As of now it might be easiest to start of with simply splitting on black space as well as comments handling.
 And then if i every feel like it, variable handling and piping and wildcard handling. Where wildcard will run everything that has the following text. So if i turn in *.txt it will expand into all text files.



 Make a recalloc so that i can dynamically reallocate the memory if my buffer when needed.
 To do tomorrow: Create switch case for my parser. Create unit tests and a makefile that will compile everything, run tests, return results and then run the shell program.

 NitPickings: When switching between past command lines written. AA> to signify that we are in the shell gets reset as well. Which is simply unnecessary. It is because i clear the whole line and it should be an easy fix but it is mainly a graphical annoyance that doesn't affect anything else so i will fix it after i am done with simple parts of everything else. It is also not really noticed the majority of time on powershell. For some reason it seems like MSYS terminal is slower

 Have started learning about launching processes in windows.