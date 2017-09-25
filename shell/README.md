# Compiling and Running

To compile and run the program use the make command in the shell diretory. The make command also runs the shell right away.
To run the shell again after it was already compiled use the ./shell command.

# Using the program

Once the shell is running the shell will prompt you that it has started.
The program will also prompt you to enter a keyboard input using the '$' symbol. Once the '$' symbol is there you can start entering commands.
Before the prompt is given however, the shell tokenizes the PATH variable so it can use the paths to find programs.
Once a command is entered the program will use the function mytoc() to return a double pointer containing all the tokens in the input command.
The first token in the double pointer is used as the command and the following tokens as the arguments.
The shell first tests if the first token is a valid command by trying to execute it. If it fails it then concatinates the paths tokenized to the beginning of the command. It then tries again. When all paths are exhausted and no command executed properly then the shell prints 'Command not found'.
To exit the shell enter 'exit'.

# Files in directory

shell.c

Contains the functions myStrCmp(), strCmp(), subStr2(), strConcat() and main(). This main function has the while loop that drives the program until it is prompted to exit. This file includes the mytoc.h file to use the mytoc function.
The strCmp() function compares two string from the beginning until the number of characters specified is reached. This function is used to find the PATH var in **envp.
The subStr2() function is the same as subStr() in mytoc.c except that it terminates the string with a newline character. This is done so the tokenizer can recognize the end of an input.
The strConcat() function is used to concatinate two string and add a '/' character in the middle. This function is used to create a proper path using the paths in the PATH variable.
The function myStrCmp() was derived from code found at https://stackoverflow.com/questions/22973670/creating-my-own-strcmp-function-in-c. 

mytoc.c

Contains the fnctions subStr(), getNumberOfTokens(), and mytoc().
The function subStr() was derived from code found in strcopy.c found in the directory printDemo. subStr() returns a copy of a substing.
The function getNumberOfTokens() looks through the input string and counts the number of tokens in that string. This function is used to know how much space to allocate.
The function mytoc() breaks the input string into into several char arrays each array being one token.

mytoc.h

Header file that contains the function mytoc().

Makefile

Makefile used to compile and run the program.



