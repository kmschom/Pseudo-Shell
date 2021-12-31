# Pseudo-Shell
CIS 415 Project 1: a single-threaded UNIX system command-line interface

Implement a pseudo-shell that is capable of executing a number of UNIX system commands. 

Executes in file mode or interactive mode.
1) Interactive Mode: run by typing ./pseudo-shell. 
   Should see shell-like command line interface that is ready to receive commands.
   User interacts with the shell by typing in commands one line at a time and pressing enter. 
   Shell then executes those commands and displays any output if necessary.
2) File Mode: run by typing: ./pseudo-shell -f <filename> 
   where <filename> is the name of the batch file we wish to read commands from.
   Input file will contain lines of text, where each line represents either a single command to be executed or a sequence of commands to be executed. 
   Generates a file “output.txt” with all the same STDOUT as running in interactive mode. 
