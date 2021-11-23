#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <string.h>
#include "shell.h"
#include <sys/wait.h>


char** parse_args(char* line) {
	char** arr_args = calloc(5, sizeof(char *));
	char* token;

	int i;
	for(i = 0; i < sizeof(line); i++) {
		token = strsep(&line, " ");
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

  //The program exists after the execution of this code. To be fixed later ...
  char line[50];

  fgets(line, sizeof(line), stdin);
  int newline = strcspn(line, "\n");
  line[newline] = '\0';

  printf("\n");
  char** args = parse_args(line);
  execvp(args[0], args);
}

void runCommand(char ** args){
	int f = fork();
	if(f){
		wait(NULL);
		}
 	else {
		execvp(args[0], args);
	}
}
