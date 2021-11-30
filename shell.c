#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <string.h>
#include "shell.h"
#include "redirections.h"
#include <sys/wait.h>

void operate(){
  char * line = readline();
  char** commands = parse_semis(line);
  int i = 0;
  while(commands[i]) {
    char special = specialCharacter(commands[i]);
    if(special == '<') {
      inputRedirection(commands[i]);
    } else if(special == '>') {
      outputRedirection(commands[i]);
    } else if (special == 1) {
      appendRedirection(commands[i]);
    } else if(special == '|') {
      piping(commands[i]);
    }
    else {
      char** args = parse_args(commands[i]);
      runCommand(args);
    }
    i++;
  }
}

void print_prompt() {
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

char * readline(){
  char * line = calloc(200, sizeof(char));
  fgets(line, 200, stdin);
  int newline = strcspn(line, "\n");
  line[newline] = '\0';
  return line;
}

char** parse_semis(char* line) {
	char** arr_args = calloc(50, sizeof(char *));
	char* token = line;
  int i;
	for(i = 0; i < sizeof(arr_args) && token != NULL; i++) {
		token = strsep(&line, ";");
		arr_args[i] = token;
	}
	return arr_args;
}

char** parse_args(char* line) {
	char** arr_args = calloc(50, sizeof(char *));
	char* token = line;

	int i;
	for(i = 0; i < sizeof(arr_args) && token != NULL;) {
		token = strsep(&line, " ");
    if(token && strlen(token)){
      arr_args[i] = token;
      i++;
    }
	}

	return arr_args;
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
      exit(0);
		}
	}
}
