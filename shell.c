#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <string.h>
#include "shell.h"
#include <sys/wait.h>


char** parse_args(char* line) {
	char** arr_args = calloc(50, sizeof(char *));
	char* token;

	int i;
	for(i = 0; i < sizeof(line); i++) {
		token = strsep(&line, " ");
		arr_args[i] = token;
	}

	return arr_args;
}

//Same idea as parse_args but with semicolons.
char** parse_semis(char* line) {
	char** arr_args = calloc(50, sizeof(char *));
	char* token;

	int i;
	for(i = 0; i < sizeof(line); i++) {
		token = strsep(&line, ";");
		arr_args[i] = token;
	}

	return arr_args;
}

void setup() {
  char* user = getenv("USER");

  long size;
  char* buff;
	char* cwd;

  size = pathconf(".", _PC_PATH_MAX);
  buff = (char *)malloc((size_t)size);
  if(buff != NULL) {
    cwd = getcwd(buff, (size_t)size);
  }

  printf("%s:", user);
  printf("%s$ ", cwd);
}



void runCommand(char ** args) {
	if(strcmp(args[0], "exit") == 0) {
		exit(0);
	}
	else if(strcmp(args[0], "cd") == 0) {
		chdir(args[1]);
	}
	else {
		int f = fork();

		if(f) {
			wait(NULL);
		}
		else {
			execvp(args[0], args);
		}
	}
}
