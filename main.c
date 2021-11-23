#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <string.h>
#include "shell.h"
#include <sys/wait.h>

int main() {
  while(1){
    setup();
    char line[50];

    printf("Enter commands: \n");
    fgets(line, sizeof(line), stdin);
    int newline = strcspn(line, "\n");
    line[newline] = '\0';
    printf("\n");
    char** args = parse_args(line);
    int status, f;
    f = fork();
    if(f){
      wait(&status);
      if(WIFEXITED(status)){
        continue;
      }
    } else {
      execvp(args[0], args);
    }
}
  return 0;
}
