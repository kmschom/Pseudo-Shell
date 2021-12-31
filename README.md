# Pseudo-Shell
CIS 415 Project 1: a single-threaded UNIX system command-line interface

Implement a pseudo-shell that is capable of executing a number of UNIX system commands. 

Project Details in Project1.pdf

The creator of each file is noted at the top of the file along with it's date of creation.

main.c: This file contains the main function.
   Must be able to start the program in both file and interactive mode.
   The main function has the following input parameters: argc and argv
      argc contains the number of input parameters for the main function.
      argv contains a 2D array of strings that correspond to the parameters input into the program on execution. (e.g. the filename)
   Uses a loop that reads commands on a line-by-line basis using getline() from the command line or a file.
   Parses the input string command into tokens using provided header file to identify the commands and their arguments.
   
command.h: A header file containing all function names that execute Unix commands.

string_parser.h: A header file that contains user-defined command_line struct and functions headers related to them.

string_parser.c: This file contains all the functions for the string_parser header file.

command.c: This file contains the function definitions for shell commands.

ToDo:
cp command does not work
<mv file dir> command does not work
