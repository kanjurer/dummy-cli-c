#ifndef __HISTORY_HEADER
#define __HISTORY_HEADER

typedef struct
{
    int MAX_SIZE;
    void **commands;
    void **top;
} History;

typedef struct
{
    int pid;
    char *commandString;
} Command;

History *initialize_history();
void add_command(History *, int, char *);
void enumerate_history(History *);
void _print_command(int, Command *);
Command *get_command_at_index(History *, int);
void destroy_history(History *);

#endif