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

    //Supposed to parse the line from stdin by semicolons if there are any and
    //then parse those arrays by spaces. Each sub-array is then supposed to be
    //execvp'd. Not working as expected, but the program does not pass an error
    //when a semicolon is present in the line, it just ignores the substrings
    //that follow it.
    char** args = parse_semis(line);
    for(int i = 0; i < count_semis(line); i++) {
      char** commands = parse_args(args[i]);
      otherCommands(args);
      runCommand(args);
    }

    args = parse_args(line);
    otherCommands(args);
    runCommand(args);
  }
  return 0;
}
