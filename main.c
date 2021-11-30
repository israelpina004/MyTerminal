#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <string.h>
#include "shell.h"
#include "redirections.h"
#include <sys/wait.h>

int main() {
  printf("\nWelcome to Aaron and Israel's shell.\n\n");

  while(1){
    setup();
    char * line = readline();
    char** args = parse_semis(line);
    int i = 0;
    while(args[i] != NULL) {
      char special = specialCharacter(args[i]);
      if(special == '<') {
        inputRedirection(args[i]);
      } else if(special == '>') {
        outputRedirection(args[i]);
      } else if (special == 1) {
        appendRedirection(args[i]);
      } else if(special == '|') {
        piping(args[i]);
      }
      else {
        char** commands = parse_args(args[i]);
        runCommand(commands);
      }
      i++;
    }

    printf("-------------------------------------------------------------------------------------\n");
  }

  return 0;
}
