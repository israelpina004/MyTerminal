#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>
#include "shell.h"
#include "redirections.h"
#include <sys/wait.h>

void inputRedirection(char *line){
  char * file = strsep(&line, "<");
  int new_input = open(file, O_RDONLY, 0644);
  int copy_of_input = dup(STDIN_FILENO);
  dup2(new_input,STDIN_FILENO);
  operate();
  dup2(copy_of_input,STDIN_FILENO);
  close(copy_of_input);
  close(new_input);
}

void outputRedirection(char *line){
  char * path = line;
  strsep(&path, ">");
  int i = 0;
  while(path[i]==' '){
    path++;
    i++;
  }
  int new_output = open(path, O_WRONLY|O_CREAT, 0644);
  int copy_of_output = dup(STDOUT_FILENO);
  dup2(new_output,STDOUT_FILENO);
  char ** args = parse_args(line);
  runCommand(args);
  dup2(copy_of_output,STDOUT_FILENO);
  close(new_output);
  close(copy_of_output);
}

void appendRedirection(char * line){
  char * path = strsep(&line, ">>");
  int i = 0;
  while(line[i]==' '){
    line++;
    i++;
  }
  int new_output = open(line, O_APPEND);
  int copy_of_output = dup(STDOUT_FILENO);
  dup2(new_output,STDOUT_FILENO);
  char ** args = parse_args(path);
  runCommand(args);
  dup2(copy_of_output,STDOUT_FILENO);
  close(new_output);
  close(copy_of_output);
}
void piping(char *line){
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
  }

  char input[500];
  while(fgets(input, sizeof(input), fd1)) {
    fputs(input, fd2);
  }

  pclose(fd1);
  pclose(fd2);
  free(pipe_args);
}
