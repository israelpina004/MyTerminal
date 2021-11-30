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
    operate();
    printf("-------------------------------------------------------------------------------------\n");
  }

  return 0;
}
