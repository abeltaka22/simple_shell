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
char *read_line()
{
char *line = NULL;
size_t bufsize = 0;

getline(&line, &bufsize, stdin);
return (line);
}
/**
*parse_line- function to parse the input line and split it into tokens
*@line: the input line to be parsed
* Return: An array of strings (tokens) representing the parsed input
*/
char **parse_line(char *line)
{
int bufsize = BUFFER_SIZE, position = 0;
char **tokens = malloc(bufsize * sizeof(char *));
char *token;

if (!tokens)
{
perror("Memory allocation error");
exit(EXIT_FAILURE);
}
token = strtok(line, "\t\n\r\a");
while (token != NULL)
{
tokens[position] = token;
position++;
if (position >= bufsize)
{
bufsize += BUFFER_SIZE;
tokens = realloc(tokens, bufsize *sizeof(char *));
if (!tokens)
{
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
*Return: 1 if shell should continue execution, 0 if the "exit" command is used
*/
int execute_builtin(char **args)
{
if (strcmp(args[0], "cd") == 0)
{
if (args[1] == NULL)
{
fprintf(stderr, "cd: expected argument to \"cd\"\n");
}
else
{
if (chdir(args[1]) != 0)
{
perror("cd");
}
}
return (0);
}
else if (strcmp(args[0], "exit") == 0)
{
return (0);
}
return (-1);
}

/**
* execute_command- Function to execute an external command
* @args: array of strings with command and its arguments
* Return: 1 shell to continue execution, 0 if exit command used, -1 if* error.
*/
int execute_command(char **args)
{
pid_t pid;
int status;
int interactive;/*assume interactive mode by default*/
interactive = 1;
/*check if the input is coming from stdin (non-interactive)*/
if (!isatty(STDIN_FILENO))
{
interactive = 0;
}
pid = fork();

if (pid == 0)
{
char *command = args[0];
char **argv = (char **)malloc(2 * sizeof(char *));

argv[0] = command;
argv[1] = NULL;

if (execvp(command, argv) == -1)
{
perror("Error");
free(argv);
exit(EXIT_FAILURE);
}
}
else if (pid < 0)
{
perror("Error");
}
else
{
if (interactive)
{/*wait for the child process to complete (interactive mode)*/
do {
waitpid(pid, &status, WUNTRACED);
} while (!WIFEXITED(status) && !WIFSIGNALED(status));
}
else
{/**
*for non-interactive mode, we don't wait for the child process,
*instead, we return immediately so that the shell can continue
*/
return (1);
}
}
return (1);
}
/**
* execute_commands_from_stdin - Function to read and execute commands from standard input in
non-interactive mode.
*/
void execute_commands_from_stdin(void)
{
char *line;
char **args;
int status = 1;

do {
line = read_line();
args = parse_line(line);

if (args[0] != NULL)
{
int builtin_status = execute_builtin(args);

if (builtin_status == -1)
{
status = execute_command(args);
}
else if (builtin_status == 0)
{
status = 0;
}
}
free(line);
free(args);
} while (status);
}
/**
 * main - Entry point of the program.
 *@argc: The count of arguments passed to the function.
*@argv:  An array of arguments passed to the function.
 * Return: Always 0.
 */
int main(int argc, char **argv)
{
char *line;
char **args;
int status;
status = 1;

if (argc == 1)
{/*interactive mode*/
while  (status)
{
printf("($) ");
line = read_line();
args = parse_line(line);

if (args[0] != NULL)
{
int builtin_status = execute_builtin(args);

if (builtin_status == -1)
{
status = execute_command(args);
}
else if (builtin_status == 0)
{
status = 0;
}
}
free(line);
free(args);
} while (status);
}
else if (argc == 2)
{/*arguments provided, run the shell in non-interactive move*/
FILE *file = fopen(argv[1], "r");
if (file == NULL)
{
perror("Error opening file");
return (1);
}
/*redirect standard input to the file*/
dup2(fileno(file), STDIN_FILENO);
fclose(file);
/*execute commands from standard input*/
execute_commands_from_stdin();
}
else
{
fprintf(stderr, "Usage: %s [script]\n", argv[0]);
return (1);
}
return (0);
}
