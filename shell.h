#ifndef SHELL_H
#define SHELL_H

#include <stdio.h> /* for printf*/
#include <unistd.h> /* for fork, execve*/
#include <stdlib.h>
#include <string.h> /* for strtok*/
#include <stddef.h>
#include <errno.h> /* for errno and perror */
#include <sys/types.h> /* for type pid */
#include <sys/wait.h> /* for wait */
#include <sys/stat.h> /* for use of stat function */
#include <signal.h> /* for signal management */
#include <fcntl.h> /* for open files*/

/************* MACROS **************/

/************* STRUCTURES **************/

/**
 * struct info- struct for the program's data
 * @program_name: the name of the executable
 * @input_line: pointer to the input read for _getline
 * @command_name: pointer to the first command typed by the user
 * @exec_counter: number of excecuted comands
 * @file_descriptor: file descriptor to the input of commands
 * @tokens: pointer to array of tokenized input
 * @env: copy of the environ
 * @alias_list: array of pointers with aliases.
 */
typedef struct info
{
	char *program_name;
	char *input_line;
	char *command_name;
	int exec_counter;
	int file_descriptor;
	char **tokens;
	char **env;
	char **alias_list;
} data_of_program;

/**
 * struct builtins - struct for the builtins
 * @builtin: the name of the builtin
 * @function: the associated function to be called for each builtin
 */
typedef struct builtins
{
	char *builtin;
	int (*function)(data_of_program *data);
} builtins;


/************* MAIN FUNCTIONS *************/


/**
 * execute_commands_from_stdin - Function to read and execute commands from standard input in non-interactive mode.
 */
void execute_commands_from_stdin(void)
{
    char *line;
    char **args;
    int status = 1;

    do
    {
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

/*========  shell.c  ========*/

/* Inicialize the struct with the info of the program */
void inicialize_data(data_of_program *data, int arc, char *argv[], char **env);

/* Makes the infinite loop that shows the prompt*/
void sisifo(char *prompt, data_of_program *data);

/* Print the prompt in a new line */
void handle_ctrl_c(int opr UNUSED);


/*========  _getline.c  ========*/

/* Read one line of the standar input*/
int _getline(data_of_program *data);

/* split the each line for the logical operators if it exist */
int check_logic_ops(char *array_commands[], int i, char array_operators[]);


/*======== expansions.c ========*/

/* expand variables */
void expand_variables(data_of_program *data);

/* expand aliases */
void expand_alias(data_of_program *data);

/* append the string to the end of the buffer*/
int buffer_add(char *buffer, char *str_to_add);


/*======== str_tok.c ========*/

/* Separate the string in tokens using a designed delimiter */
void tokenize(data_of_program *data);

/* Creates a pointer to a part of a string */
char *_strtok(char *line, char *delim);


/*======== execute.c ========*/

/* Execute a command with its entire path */
int execute(data_of_program *data);


/*======== builtins_list.c ========*/

/* If match a builtin, executes it */
int builtins_list(data_of_program *data);


/*======== find_in_path.c ========*/

/* Creates an array of the path directories */
char **tokenize_path(data_of_program *data);

/* Search for program in path */
int find_program(data_of_program *data);


/************** HELPERS FOR MEMORY MANAGEMENT **************/


/*======== helpers_free.c ========*/

/* Frees the memory for directories */
void free_array_of_pointers(char **directories);

/* Free the fields needed each loop */
void free_recurrent_data(data_of_program *data);

/* Free all field of the data */
void free_all_data(data_of_program *data);


/************** BUILTINS **************/


/*======== builtins_more.c ========*/

/* Close the shell */
int builtin_exit(data_of_program *data);

/* Change the current directory */
int builtin_cd(data_of_program *data);

/* set the work directory */
int set_work_directory(data_of_program *data, char *new_dir);

/* show help information */
int builtin_help(data_of_program *data);

/* set, unset and show alias */
int builtin_alias(data_of_program *data);


/*======== builtins_env.c ========*/

/* Shows the environment where the shell runs */
int builtin_env(data_of_program *data);

/* create or override a variable of environment */
int builtin_set_env(data_of_program *data);

/* delete a variable of environment */
int builtin_unset_env(data_of_program *data);


/************** HELPERS FOR ENVIRONMENT VARIABLES MANAGEMENT **************/


/*======== env_management.c ========*/

/* Gets the value of an environment variable */
char *env_get_key(char *name, data_of_program *data);

/* Overwrite the value of the environment variable */
int env_set_key(char *key, char *value, data_of_program *data);

/* Remove a key from the environment */
int env_remove_key(char *key, data_of_program *data);

/* prints the current environ */
void print_environ(data_of_program *data);


/************** HELPERS FOR PRINTING **************/


/*======== helpers_print.c ========*/

/* Prints a string in the standar output */
int _print(char *string);

/* Prints a string in the standar error */
int _printe(char *string);

/* Prints a string in the standar error */
int _print_error(int errorcode, data_of_program *data);


/************** HELPERS FOR STRINGS MANAGEMENT **************/


/*======== helpers_string.c ========*/

/* Counts the number of characters of a string */
int str_length(char *string);

/* Duplicates an string */
char *str_duplicate(char *string);

/* Compares two strings */
int str_compare(char *string1, char *string2, int number);

/* Concatenates two strings */
char *str_concat(char *string1, char *string2);

/* Reverse a string */
void str_reverse(char *string);


/*======== helpers_numbers.c ========*/

/* Cast from int to string */
void long_to_string(long number, char *string, int base);

/* convert an string in to a number */
int _atoi(char *s);

/* count the coincidences of character in string */
int count_characters(char *string, char *character);


/*======== alias_management.c ========*/

/* print the list of alias */
int print_alias(data_of_program *data, char *alias);

/* get the alias name */
char *get_alias(data_of_program *data, char *alias);

/* set the alias name */
int set_alias(char *alias_string, data_of_program *data);

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
* @args: An array of strings containing the command and its arguments
* Return: 1 if shell should continue execution, 0 if the "exit" command is used or -1 if error occours.
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
if (interactive) {/*wait for the child process to complete (interactive mode)*/
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
      

#endif /* SHELL_H */
