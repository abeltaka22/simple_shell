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
        char *line = NULL;
        size_t bufsize = 0;
        getline(&line, &bufsize, stdin);
        return line;
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
* execute_builtin- Function to execute built in commands
* @args: An array of strings containing the command and its arguments
*Return: 0 if the "exit" command is used, 1 if the "env" command is used, otherwise -1
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
                return (-1);
        } else if (strcmp(args[0], "exit") == 0) {
                return (0); /*return 0 to indicate the "exit" command*/
        } else if (strcmp(args[0], "env") == 0) {/*execute the "env" command to print the environment*/
	char **env = environ;
	while (*env) {
		printf("%s\n", *env);
		env++;
	}
        return (1);
}
return (-1);
}
/**
* execute_command- Function to execute an external command
* @args: An array of strings containing the command and its arguments
*@envp: The environment variables including the PATH
* Return: 1 if the shell should continue execution, 0 if the "exit" command is used
*/
int execute_command(char **args, char **envp) {
char *command = argr[0];
int status = 1;
/* check if the command exists in the path*/
char command_path[BUFFER_SIZE];
int i = 0;
while (envp[i] != NULL)[
if (strncmp(envp[i], "PATH=", 5) ==0) {
        char *path = envp[i] + 5;
        char *path_token = strtok(path, ":");
        while (path_token != NULL) {
        snprintf(command_path, sizeof(command_path), "%s/%s", path_token, command);
        if (access(command_path, X_OK) == 0) {
        /*command found in the PATH, execute it*/
pid_t pid;
pid = fork();
if (pid == 0) {
                if (execvp(command_path, args) == -1) {
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
        break;
}
path_token = strtok(NULL, ":");
}
break;
}
i++;
}
if (envp[i] == NULL) {
        fprintf(stderr, "%s: command not found\n", command);
}
return (1);
}
/**
 * main - Entry point of the program.
 *@argc: The number of command-line arguments.
*@argv: An array of strings containing the command-line arguments.
*@envp: The environment variables.
 * Return: 0 on successful completion.
 */
int main(int argc, char *argv[], char *envp[]) {
        char *line;
        char **args;
        int status;

        do {
                printf(":) ");
                line = read_line();
                args = parse_line(line);

                if (args[0] != NULL) {
                        int builtin_status = execute_builtin(args);
                        if (builtin_status == 0) {/*exit the shell when the exit built-in is used*/
                break;
                } else if(builtin_status == -1) {
                                status = execute_command(args, envp);
                                }
                                }

                free(line);
                free(args);
        } while (status);

        return (0);
}
