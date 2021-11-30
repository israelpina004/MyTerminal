#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <string.h>
#include "shell.h"
#include "redirections.h"
#include <sys/wait.h>

//How the shell runs; operates all the functions necessary to run the shell. It
//first checks for semicolons, then for "special characters" (pipes, redirection),
//performs the appropriate task if these special characters are present, then
//runs any built-in commands.
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

//Prints the user's path and username, mimicking the format of a real shell.
//Also reads from stdin the user's commands.
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

//Takes in the user's commands from stdin as an argument and returns an array of
//strings. Each substring is spliced through the semicolons in between them and
//are stored in arr_args. These substrings are then passed through parse_args.
//This mimics the functionality of semicolons in actual shells.
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

//Takes in a string from the array of strings returned from parse_semis as an
//argument and returns an array of strings for execvp to use. Each substring is
//spliced through the spaces in between them and are stored in arr_args.
char** parse_args(char* line) {
	char** arr_args = calloc(50, sizeof(char *));
	char* token = line;

	int i;
	for(i = 0; i < sizeof(arr_args) && token != NULL;) {
		token = strsep(&line, " ");
    if(token && strlen(token)) {
      arr_args[i] = token;
      i++;
    }
	}

	return arr_args;
}

//Helps the shell check for redirection or pipes. If it does, it goes on to
//perform the appropriate functions depending on which special character is
//detected.
char specialCharacter(char * line){
 if(strchr(line, '<')) {
		return '<';
	}
  else if(strstr(line, ">>")) {
		return 1;
	}
  else if(strchr(line, '>')) {
		return '>';
	}
  else if(strchr(line, '|')) {
		return '|';
	}
  else {
		return 0;
	}
}

//Runs the user's command(s), forking first and then taking in the array of
//strings from parse_args and passing them through execvp. Checks first if the
//command is not a built-in command (cd, exit).
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
