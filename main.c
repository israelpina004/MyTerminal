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
    char line[100];
    fgets(line, sizeof(line), stdin);
    int newline = strcspn(line, "\n");
    line[newline] = '\0';
    printf("\n");
    char** args = parse_args(line);
    int f = fork();
    printf("f\n");
  	if(f){
  		wait(NULL);
  		printf("hi\n");
  		}
   	else {
  		execvp(args[0], args);
    }
  }
  return 0;
}
