/*
 * command.c
 *
 * Created on: Oct 15, 2021
 *     Author: Kelly Schombert
 *
 * Provides implementation of commands from command.h
 */

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include "command.h"
#include "string_parser.h"

void listDir() /*for the ls command*/
{
	char cwd[300];
	char *currentDir;
	char *buf = NULL;

	currentDir = getcwd(cwd, sizeof(cwd));

	DIR *dr;
	dr = opendir(cwd);
	struct dirent *de;

	if (dr == NULL) 
	{
		buf = "Could not open current directory";
		write(1, buf, strlen(buf));
		return;
	}

	while ((de = readdir(dr)) != NULL)
	{
		buf = de->d_name;
		write(1, buf, strlen(buf));
		buf = " ";
		write(1, buf, strlen(buf));
	}
	buf = "\n";
	write(1, buf, strlen(buf));

	closedir(dr);
	return;
}

void showCurrentDir() /*for the pwd command*/
{
	char cwd[300];
	char *currentDir;
	char *buf = NULL;

	currentDir = getcwd(cwd, sizeof(cwd));
	write(1, currentDir, strlen(currentDir));
	buf = "\n";
	write(1, buf, strlen(buf));
}

void makeDir(char *dirName) /*for the mkdir command*/
{
	int check;
	check = mkdir(dirName, 0777);
	char *buf = NULL;

	if(check == -1)
	{
		buf = "Error: ";
		write(1, buf, strlen(buf));
		write(1, strerror(errno), strlen(strerror(errno)));
		buf = "\n";
		write(1, buf, strlen(buf));
	}

	return;
}

void changeDir(char *dirName) /*for the cd command*/
{
	int check;
	check = chdir(dirName);
	char *buf = NULL;

	if(check == -1)
	{
		buf = "Error: ";
		write(1, buf, strlen(buf));
		write(1, strerror(errno), strlen(strerror(errno)));
		buf = "\n";
		write(1, buf, strlen(buf));
	}
	return;
}

void copyFile(char *sourcePath, char *destinationPath) /*for the cp command*/
{
	/* My failed attempt
	 * freopen(destinationPath, "w+", stdout);
	
	int fd_read;
	char line_buf[1024];
	char *buf = NULL;
	ssize_t count;
	//fd_read = open(sourcePath, O_RDONLY);

	//if(fd_read == -1)
	//{
	//	buf = "Error: ";
	//	write(1, buf, strlen(buf));
	//	write(1, strerror(errno), strlen(strerror(errno)));
	//	buf = "\n";
	//	write(1, buf, strlen(buf));
	//}

	//while((count = read(fd_read, line_buf, 1024)) >= 0)
	//{
	//	write(1, line_buf, count);
	//}

	//close(fd_read);
	return;
	*/

	//NOTE: This code is from Brianna Vago and was not my own code
	//This code was added after this project was submitted.
	//I did not pass this code off as my own
	
	//new destination
	int dest_len = strlen(destinationPath);
	char new_dest[dest_len];
	memset(new_dest, '\0', sizeof(char)*dest_len);

	//new source
	int source_len = strlen(sourcePath);
	char new_source[source_len];
	memset(new_source, '\0', sizeof(char)*source_len);

	//final destination
	char final_dest[source_len + dest_len];
	memset(final_dest, '\0', sizeof(char)*(source_len + dest_len));

	//for loop through lenth of source path
	for(int i=0; i<source_len; i++)
	{
		//if char in source path isn't an endline character than put in new source
		if(strcmp(&sourcePath[i], "\n") != 0)
		{
			strncat(new_source, &sourcePath[i], 1);
		}
	}

	//for loop through length of destination path
	for(int i = 0; i<dest_len; i++)
	{
		//if char in destination path isn't an endline char then put in new destination
		if(strcmp(&destinationPath[i], "\n") != 0)
		{
			strncat(new_dest, &destinationPath[i], 1);
		}
	}

	ssize_t chars;
	char* buf = malloc(sizeof(char)*255);
	char* cwd = malloc(sizeof(char)*255);
	int s = open(new_source, O_RDONLY);
	getcwd(cwd, 255);

	char* dest_name;
	char* name;

	//if new destination ins't a dot
	if (strcmp(new_dest, ".") != 0)
	{
		//set new to a tokenized version of new destination
		char* new = strtok(new_dest, "/");
		//while new isn't NULL, set name to neww and set new to token
		while(new != NULL) 
		{
			name = new;
			new = strtok(NULL, "/");
		}
	}
	else
	{
		//set new to a token version of new source
		char* new = strtok(new_source, "/");
		//while new isn't NULL, set name to new and set new to token
		while(new != NULL)
		{
			name = new;
			new = strtok(NULL, "/");
		}
	}
	//change dir to final destination
	chdir(final_dest);

	//open name file
	int dest = open(name, O_CREAT);
	chmod(name, 0770);
	close(dest);
	//read source
	chars = read(s, buf, 255);
	//open name files write only
	dest = open(name, O_WRONLY);
	//while chars is more than zero write to the file
	while(chars > 0) 
	{
		write(dest, buf, chars);
		chars = read(s, buf, 255);
	}

	//free and close allocated vars
	free(buf);
	free(cwd);
	close(dest);
	close(s);
}

void moveFile(char *sourcePath, char *destinationPath) /*for the mv command*/
{
	int check;
	check = rename(sourcePath, destinationPath);
	char *buf = NULL;

	if(check == -1)
	{
		buf = "Error: ";
		write(1, buf, strlen(buf));
		write(1, strerror(errno), strlen(strerror(errno)));
		buf = "\n";
		write(1, buf, strlen(buf));
	}
	return;
}

void deleteFile(char *filename) /*for the rm command*/
{
	int check;
	check = unlink(filename);
	char *buf = NULL;

	if(check == -1)
	{
		buf = "Error: ";
		write(1, buf, strlen(buf));
		write(1, strerror(errno), strlen(strerror(errno)));
		buf = "\n";
		write(1, buf, strlen(buf));
	}
	return;
}

void displayFile(char *filename) /*for the cat command*/
{
	int fd_read;
	char line_buf[1024];
	char *buf = NULL;
	ssize_t count;
	fd_read = open(filename, O_RDONLY);

	if(fd_read == -1)
	{
		buf = "Error: ";
		write(1, buf, strlen(buf));
		write(1, strerror(errno), strlen(strerror(errno)));
		buf = "\n";
		write(1, buf, strlen(buf));
	}

	while((count = read(fd_read, line_buf, 1024)) > 0)
	{
		write(1, line_buf, count);
	}
	
	close(fd_read);
	return;
}
