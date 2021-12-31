/*
 * main.c
 *
 * Created on: Oct 15, 2021
 *     Author: Kelly Schombert
 *
 * Part of ./pseudo-shell.exe
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include "string_parser.h"
#include "command.h"

int main(int argc, char const *argv[])
{
	// Have prog determine whether it's running interactive 
	// or file mode by detecting a -f flag or not.

	// interactive mode should only have one arg and 
	// file mode should have a -f flag
	if ((argc != 1) && (strcmp(argv[1], "-f") != 0))
	{
		printf ("Incorrect usage of ./pseudo-shell.exe: Indicate file mode with -f flag\n");
		return(1);
	}

	// if running file mode, open or make the file given 
	// as second arg to write to later
	int fmode = 0;
	if ((argc != 1) && (strcmp(argv[1], "-f") == 0))
	{
		freopen("output.txt", "w+", stdout);
		fmode = 1;
	}

	char *line_buf = NULL;
	size_t len = 0;
	command_line large_token_buffer;
	command_line small_token_buffer;

	if (fmode == 0) // interactive mode main loop
	{
		char *error = NULL;
		char *token = NULL;
		printf(">>> ");
		
		while(getline(&line_buf, &len, stdin) != -1)
		{
			//tokenize line buffer; large token separated by ";"
			large_token_buffer = str_filler(line_buf, ";");

			for (int i = 0; large_token_buffer.command_list[i] != NULL; i++)
			{
				//tokenize small buffer; separated by " "
				small_token_buffer = str_filler(large_token_buffer.command_list[i], " ");
				for (int j = 0; small_token_buffer.command_list[j] != NULL; j++)
				{
					token = small_token_buffer.command_list[j];
					
					if(strcmp(token, "exit") == 0)
					{
						//free smaller tokens and reset variable
						free_command_line(&small_token_buffer);
						memset(&small_token_buffer, 0, 0);
						//free larger tokens and reset variable
						free_command_line(&large_token_buffer);
						memset(&large_token_buffer, 0, 0);
						free(line_buf);
						return 0;
					}
					else if(strcmp(token, "ls") == 0)
					{
						if(small_token_buffer.command_list[j+1] != NULL)
						{
							error = "Error! Unsupported parameters for command: ls\n";
							write(1, error, strlen(error));
							j++;
							break;
						}else
							listDir();
					}
					else if(strcmp(token, "pwd") == 0)
					{
						if(small_token_buffer.command_list[j+1] != NULL)
						{
							error = "Error! Unsupported parameters for command: pwd\n";
							write(1, error, strlen(error));
							j++;
							break;
						}else
							showCurrentDir();
					}
					else if(strcmp(token, "mkdir") == 0)
					{
						if(small_token_buffer.command_list[j+1] == NULL)
						{
							error = "Error! Need 1 parameter for command: mkdir\n";
							write(1, error, strlen(error));
							break;
						}else{
							makeDir(small_token_buffer.command_list[j+1]);
							j++;
						}
					}
					else if(strcmp(token, "cd") == 0)
					{
						if(small_token_buffer.command_list[j+1] == NULL)
						{
							error = "Error! Need 1 parameter for command: cd\n";
							write(1, error, strlen(error));
							break;
						}else{
							changeDir(small_token_buffer.command_list[j+1]);
							j++;
						}
					}
					else if(strcmp(token, "cp") == 0)
					{
						if(small_token_buffer.command_list[j+2] == NULL)
						{
							error = "Error! Need 2 parameters for command: cp\n";
							write(1, error, strlen(error));
							j++;
							break;
						}else{
							copyFile(small_token_buffer.command_list[j+1], small_token_buffer.command_list[j+2]);
							j += 2;
						}
					}
					else if(strcmp(token, "mv") == 0)
					{
						if(small_token_buffer.command_list[j+2] == NULL)
						{
							error = "Error! Need 2 parameters for command: mv\n";
							write(1, error, strlen(error));
							j++;
							break;
						}else{
							moveFile(small_token_buffer.command_list[j+1], small_token_buffer.command_list[j+2]);
							j += 2;
						}
					}
					else if(strcmp(token, "rm") == 0)
					{
						if(small_token_buffer.command_list[j+1] == NULL)
						{
							error = "Error! Need 1 parameter for command: rm\n";
							write(1, error, strlen(error));
							break;
						}else{
							deleteFile(small_token_buffer.command_list[j+1]);
							j++;
						}
					}
					else if(strcmp(token, "cat") == 0)
					{
						if(small_token_buffer.command_list[j+1] == NULL)
						{
							error = "Error! Need 1 parameter for command: cat\n";
							write(1, error, strlen(error));
							break;
						}else{
							displayFile(small_token_buffer.command_list[j+1]);
							j++;
						}
					}
					else 
					{
						error = "Error! Unrecognized command: ";
						write(1, error, strlen(error));
						write(1, token, strlen(token));
						error = "\n";
						write(1, error, strlen(error));
						break;
					}
				}

				//free smaller tokens and reset variable
				free_command_line(&small_token_buffer);
				memset(&small_token_buffer, 0, 0);
			}

			//free larger tokens and reset variable
			free_command_line(&large_token_buffer);
			memset(&large_token_buffer, 0, 0);

			printf(">>> ");
		}
		
		free(line_buf);
		return 0;
	}
	else // file mode main loop
	{
		FILE *inputFile = fopen(argv[2], "r");
		if(inputFile == NULL)
		{
			printf("Error: %s does not exist. Must input name of a currently existing file.\n", argv[2]);
			return 1;
		}

		char *prompt = NULL;
		char *error = NULL;
		char *token = NULL;
		prompt = ">>> ";
		
		while (getline(&line_buf, &len, inputFile) != -1)
		{
			write(1, prompt, strlen(prompt));
			write(1, line_buf, strlen(line_buf));

			//tokenize line buffer; large token separated by ";"
			large_token_buffer = str_filler(line_buf, ";");
			
			for (int i = 0; large_token_buffer.command_list[i] != NULL; i++)
			{
				//tokenize small buffer; separated by " "
				small_token_buffer = str_filler(large_token_buffer.command_list[i], " ");
				for (int j = 0; small_token_buffer.command_list[j] != NULL; j++)
				{
					token = small_token_buffer.command_list[j];
					
					if(strcmp(token, "exit") == 0)
					{
						//free smaller tokens and reset variable
						free_command_line(&small_token_buffer);
						memset(&small_token_buffer, 0, 0);
						//free larger tokens and reset variable
						free_command_line(&large_token_buffer);
						memset(&large_token_buffer, 0, 0);
						free(line_buf);
						fclose(inputFile);
						return 0;
					}
					else if(strcmp(token, "ls") == 0)
					{
						if(small_token_buffer.command_list[j+1] != NULL)
						{
							error = "Error! Unsupported parameters for command: ls\n";
							write(1, error, strlen(error));
							j++;
							break;
						}else
							listDir();
					}
					else if(strcmp(token, "pwd") == 0)
					{
						if(small_token_buffer.command_list[j+1] != NULL)
						{
							error = "Error! Unsupported parameters for command: pwd\n";
							write(1, error, strlen(error));
							j++;
							break;
						}else
							showCurrentDir();
					}
					else if(strcmp(token, "mkdir") == 0)
					{
						if(small_token_buffer.command_list[j+1] == NULL)
						{
							error = "Error! Need 1 parameter for command: mkdir\n";
							write(1, error, strlen(error));
							break;
						}else{
							makeDir(small_token_buffer.command_list[j+1]);
							j++;
						}
					}
					else if(strcmp(token, "cd") == 0)
					{
						if(small_token_buffer.command_list[j+1] == NULL)
						{
							error = "Error! Need 1 parameter for command: cd\n";
							write(1, error, strlen(error));
							break;
						}else{
							changeDir(small_token_buffer.command_list[j+1]);
							j++;
						}
					}
					else if(strcmp(token, "cp") == 0)
					{
						if(small_token_buffer.command_list[j+2] == NULL)
						{
							error = "Error! Need 2 parameters for command: cp\n";
							write(1, error, strlen(error));
							j++;
							break;
						}else{
							copyFile(small_token_buffer.command_list[j+1], small_token_buffer.command_list[j+2]);
							j += 2;
						}
					}
					else if(strcmp(token, "mv") == 0)
					{
						if(small_token_buffer.command_list[j+2] == NULL)
						{
							error = "Error! Need 2 parameters for command: mv\n";
							write(1, error, strlen(error));
							j++;
							break;
						}else{
							moveFile(small_token_buffer.command_list[j+1], small_token_buffer.command_list[j+2]);
							j += 2;
						}
					}
					else if(strcmp(token, "rm") == 0)
					{
						if(small_token_buffer.command_list[j+1] == NULL)
						{
							error = "Error! Need 1 parameter for command: rm\n";
							write(1, error, strlen(error));
							break;
						}else{
							deleteFile(small_token_buffer.command_list[j+1]);
							j++;
						}
					}
					else if(strcmp(token, "cat") == 0)
					{
						if(small_token_buffer.command_list[j+1] == NULL)
						{
							error = "Error! Need 1 parameter for command: cat\n";
							write(1, error, strlen(error));
							break;
						}else{
							displayFile(small_token_buffer.command_list[j+1]);
							j++;
						}
					}
					else 
					{
						error = "Error! Unrecognized command: ";
						write(1, error, strlen(error));
						write(1, token, strlen(token));
						error = "\n";
						write(1, error, strlen(error));
						break;
					}
				}

				//free smaller tokens and reset variable
				free_command_line(&small_token_buffer);
				memset(&small_token_buffer, 0, 0);
			}

			//free larger tokens and reset variable
			free_command_line(&large_token_buffer);
			memset(&large_token_buffer, 0, 0);
		}

		fclose(inputFile);
		free(line_buf);
	}

	return 0;
}
