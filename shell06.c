#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

#define BUFFER_SIZE 1024

/**
* read_line- function to read a line of input from the user
* Return: line
*/
char *read_line() {
	static char buffer[BUFFER_SIZE];
	static int position = 0;
	static int char_read = 0;

        char *line = NULL;
        int bytes_read;
while (1) {/*check if we need to read more characters into the buffer
if (position >= chars_read) {
	bytes_read = read(STDIN_FILENO, buffer, BUFFER_SIZE);
	if (bytes_read <= 0) {/*End of file or error occurred, return NULL to indicate the end of input
return (NULL);
}
position = 0;
chars_read = bytes_read;
}
/*check for newline character in the buffer*/
while (position < chars_read) {
	if (buffer[position] == ‘\n’) {
		buffer[position] = ‘\0’; /*replace newline with null terminator
		line = strdup(buffer); /*duplicate the line using stdup*/
		position++;
		return (line);
		}
	position++;
	}
/*if newline character not found, allocate more memory for the line*/
int line_len = line ? strlen(line) : 0;
int new_len = line_len + chars_read – position +1;
line = realloc(line, new_len);
if (!line) {
	perror("Memory allocation error");
	exit(EXIT_FAILURE);
}
/*append characters from the buffer to the line*/
stncat(line, buffer + position – char_read, chars_read – position);
position = chars_read;
}
}

/**
*parse_line- function to parse the input line and split it into tokens
*@line: the input line to be parsed
* Return: An array of strings (tokens) representing the parsed input
*/
char **parse_line(char *line) {
        int bufsize = BUFFER_SIZE, position = 0;
        char **tokens = malloc(bufsize * sizeof(char*));
        char *token;

        if (!tokens) {
                perror("Memory allocation error");
                exit(EXIT_FAILURE);
        }

        token = strtok(line, "\t\n\r\a");
        while (token != NULL) {
                tokens[position] = token;
                position++;

                if (position >= bufsize) {
                        bufsize += BUFFER¬_SIZE;
                        tokens = realloc(tokens, bufsize * sizeof(char*));
                        if (!tokens) {
                                perror("Memory allocation error");
                                exit(EXIT_FAILURE);
                        }
                }

                token = strtok(NULL, "\t\n\r\a");
        }
        tokens[position] = NULL;
        return (tokens);
}

/**
* execute_builtin- Funton to execute built in commands
* @args: An array of strings containing the command and its arguments
*Return: 1 if the shell should continue execution, 0 if the "exit" command is used
*/
int execute_builtin(char **args) {
        if (strcmp(args[0], "cd") == 0) {
                if (args[1] == NULL) {
                        fprintf(stderr, "cd: expected argument to \"cd\"\n");
                } else {
                        if (chdir(args[1]) != 0) {
                                perror("cd");
                        }
                }
                return (1);
        } else if (strcmp(args[0], "exit") == 0) {
                return (0);
        }

        return (-1);
}

/**
* execute_command- Function to execute an external command
* @args: An array of strings containing the command and its arguments
* Return: 1 if the shell should continue execution, 0 if the "exit" command is used
*/
int execute_command(char **args) {
        pid_t pid;
        int status;

        pid = fork();
        if (pid == 0) {
                char *command = args[0];
                char *argv[] = {command, NULL};
                if (execvp(command, argv) == -1) {
                        perror("Error");
                }
                exit(EXIT_FAILURE);
        } else if (pid < 0) {
                perror("Error");
        } else {
                do {
                        wpid = waitpid(pid, &status, WUNTRACED);
                } while (!WIFEXITED(status) && !WIFSIGNALED(status));
        }

        return (1);
}
/**
 * main - Entry point of the program.
 *@argc: the number of command-line arguments
*@argv: an array of strings containing the command-line arguments
*@envp: the environment variables
*/
int main(int argc, char *argv[], char *envp[]) {
        char *line;
        char **args;
        int status;

        do {
                printf("($) ");
                line = read_line();
                args = parse_line(line);

                if (args && args[0] != NULL) {
                        int builtin_status = execute_builtin(args);
                        if (builtin_status == 0) {/*exit the shell when the exit built-in is used*/
		break;
} else if (builtin_status == -1) {
                                status = execute_command(args, envp);
                        } else if (builtin_status == 1) {/*continue shell execution after "env" built-in is used*/
                                status = 1;
                        }
                }

                free(line);
                free(args);
        } while (status);

        return (0);
}
