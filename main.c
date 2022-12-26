#include "main.h"
#include "history.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_LINE 80 /* The maximum length command */
int main(void)
{
    char *args[MAX_LINE / 2 + 1]; /* an array of character strings */
    int should_run = 1;           /* flag to determine when to exit program */
    char *input = malloc(MAX_LINE + 1);
    char *command;
    int status;

    char *f = malloc(10);

    History *history = initialize_history(10);

    while (should_run)
    {
        // scan user command
        printf("CSCI3120> ");
        scanf("%80[^\n]%c", input, f);
        fflush(stdout);

        if (strcmp(input, "exit") == 0)
        {
            should_run = 0;
        }
        else if (strcmp(input, "history") == 0)
        {
            enumerate_history(history);
        }
        else
        {
            if (strcmp(input, "!!") == 0)
            {
                Command *recentCommand = get_command_at_index(history, 1);

                if (recentCommand == NULL)
                {
                    printf("No command in history!\n");
                    continue;
                }
                else
                {
                    command = recentCommand->commandString;
                    printf("%s\n", command);
                }
            }
            else if (strlen(input) > 1 && input[0] == '!')
            {
                char *end;
                int id = strtol(input + 1, &end, 10);

                if (end == input + 1)
                {
                    command = input;
                }
                else if (id < 1 || id > 10)
                {
                    printf("Such a command is NOT in history.\n");
                    continue;
                }
                else
                {
                    Command *c = get_command_at_index(history, id);
                    if (c == NULL)
                    {
                        printf("Such a command is NOT in history.\n");
                        continue;
                    }
                    else
                    {
                        command = c->commandString;
                        printf("%s\n", command);
                    }
                }
            }
            else
            {
                command = input;
            }

            pid_t pid = fork();

            if (pid != 0)
            {
                wait(&status);
                add_command(history, pid, command);
            }
            else
            {
                argsGenerator(command, args);

                if (execvp(args[0], args) == -1)
                {
                    printf("Invalid command!\n");
                    exit(1);
                }
            }
        }
    }

    destroy_history(history);

    return 0;
}

void argsGenerator(char *str, char **args)
{
    if (str == NULL || args == NULL)
        return;

    char *s = malloc(100);

    strcpy(s, str);

    char *token = strtok(s, " \n");

    int i = 0;
    while (token)
    {
        args[i] = token;
        token = strtok(NULL, " ");
        i++;
    }

    args[i] = NULL;
}
