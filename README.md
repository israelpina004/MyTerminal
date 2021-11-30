Project 01: Getting out of your Shell

-------------------------------------------------------------------------------------------------------------------------------------------------

Features:

-Different commands can be seperated with semicolons.

-cd and exit work as appropriate.

-Different commands can be separated through semicolons.

-Piping works as appropriate between two commands.

-Simple redirection works as appropriate.

Attempts at implementing other features were not made.

-------------------------------------------------------------------------------------------------------------------------------------------------

Bugs:

-Piping only works between two commands.

-A broken pipe will forcibly exit the program.

-If the user inputs a command or commands that do not exist, it will take multiple exit calls for the program to exit.

-------------------------------------------------------------------------------------------------------------------------------------------------

Function headers:

void operate(): How the shell runs; operates all the functions necessary to run the shell. It first checks for semicolons, then for "special characters" (pipes, redirection), performs the appropriate task if these special characters are present, then runs any built-in commands.

void print_prompt(): Prints the user's path and username, mimicking the format of a real shell. Also reads from stdin the user's commands.

char** parse_semis(char* line): Takes in the user's commands from stdin as an argument and returns an array of strings. Each substring is spliced through the semicolons in between them and are stored in arr_args. These substrings are then passed through parse_args. This mimics the functionality of semicolons in actual shells.

char** parse_args(char* line): Takes in a string from the array of strings returned from parse_semis as an argument and returns an array of strings for execvp to use. Each substring is spliced through the spaces in between them and are stored in arr_args.

char specialCharacter(char * line): Helps the shell check for redirection or pipes. If it does, it goes on to perform the appropriate functions depending on which special character is detected.

void runCommand(char ** args): Runs the user's command(s), forking first and then taking in the array of strings from parse_args and passing them through execvp. Checks first if the command is not a built-in command (cd, exit).

void inputRedirection(char *line): Runs if < is found, changes stdin to a inputed file, runs operate command with arguments from file, then closes file and resets the file descriptors

void outputRedirection(char *line): Runs if > is found, changes stdout to a inputed file, runs operate command with arguments from file, then closes file and resets the file descriptors

void appendRedirection(char * line): Runs if >> is found, changes stdout to a inputed file, runs operate command with arguments from file, appending them into the file, then closes file and resets the file descriptors

void piping(char *line): Runs if | is found, forms pipe between two commands using popen
