#include "history.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

History *initialize_history(int max_size)
{
    History *history = malloc(sizeof(History));

    history->MAX_SIZE = max_size;
    history->commands = malloc(history->MAX_SIZE * sizeof(Command *));
    history->top = NULL;

    return history;
}

void add_command(History *history, int pid, char *commandString)
{
    if (pid == 0)
        return;
    if (commandString == NULL)
        return;
    if (history == NULL)
        return;

    Command *command = malloc(sizeof(Command));

    command->pid = pid;

    char *cpy = malloc(100);
    strcpy(cpy, commandString);
    command->commandString = cpy;

    if (history->top == NULL)
    {
        *((Command **)history->commands) = command;
        history->top = history->commands;
    }
    else
    {
        history->top = (history->commands) + ((history->top) - (history->commands) + 1) % history->MAX_SIZE;
        *history->top = command;
    }
}

void enumerate_history(History *history)
{
    if (history == NULL)
        return;

    if (history->top == NULL)
    {
        printf("No command in history!\n");

        return;
    }

    printf("%5s %5s %10s\n", "ID", "PID", "Command");

    Command **tmp = (Command **)history->top;
    int id = 1;
    while (*tmp != NULL && tmp != (Command **)history->commands + ((Command **)history->top - (Command **)history->commands + 1) % history->MAX_SIZE)
    {
        _print_command(id, *tmp);
        tmp = (Command **)history->commands + (history->MAX_SIZE + tmp - (Command **)history->commands - 1) % history->MAX_SIZE;
        id++;
    }

    if (*tmp != NULL)
    {
        _print_command(id, *tmp);
    }
}

void _print_command(int id, Command *command)
{
    if (command == NULL)
        return;

    printf("%5d %5d %10s\n", id, command->pid, command->commandString);
}

Command *get_command_at_index(History *history, int id)
{
    if (history == NULL)
        return NULL;
    if (history->top == NULL)
        return NULL;
    if (id < 1 || id > 10)
        return NULL;

    Command **tmp = (Command **)history->top;
    int index = 0;
    while (*tmp != NULL && tmp != (Command **)history->commands + ((Command **)history->top - (Command **)history->commands + 1) % history->MAX_SIZE)
    {
        if (index == id - 1)
            return *tmp;
        tmp = (Command **)history->commands + (history->MAX_SIZE + tmp - (Command **)history->commands - 1) % history->MAX_SIZE;
        index++;
    }

    if (*tmp != NULL && index == id - 1)
    {
        return *tmp;
    }

    return NULL;
}

void destroy_history(History *history)
{
    if (history == NULL)
        return;

    if (history->commands != NULL)
    {
        for (int i = 0; i < history->MAX_SIZE; i++)
        {
            if (history->commands[i] != NULL)
            {
                free(history->commands[i]);
                history->commands[i] = NULL;
            }
        }

        free(history->commands);
        history->commands = NULL;
    }
    history->top = NULL;

    free(history);
    history = NULL;
}