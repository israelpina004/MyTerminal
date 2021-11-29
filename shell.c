#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <string.h>
#include "shell.h"
#include "redirections.h"
#include <sys/wait.h>


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

int piping(char* line) {
  char** pipe_args = calloc(10, sizeof(char *));
  char* token;

  int i;
  for(i = 0; i < sizeof(line); i++) {
    token = strsep(&line, "|");
    pipe_args[i] = token;
  }

  FILE *fd1 = popen(pipe_args[0], "r");
  FILE *fd2 = popen(pipe_args[1], "w");
  if(fd1 == NULL || fd2 == NULL) {
    printf("Error piping.\n");
    return 1;
  }
 
  char input[500];
  while(fgets(input, sizeof(input), fd1)) {
    fputs(input, fd2);
  }
  
  pclose(fd1);
  pclose(fd2);

  return 0;
}

char specialCharacter(char * line){
	if(strchr(line, '<')) {
		return '<';
	}
  else if(strchr(line, '>')) {
		return '>';
	}
  else if(strchr(line, '|')) {
		return '|';
	}
  else if(strstr(line, ">>")) {
		return 1;
	}
  else {
		return 0;
	}
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
