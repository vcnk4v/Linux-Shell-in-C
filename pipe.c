#include "headers.h"
int checkpiping(char *command)
{
    return strchr(command, '|') != NULL;
}

int piping(char *command, int n, char *home_dir, char *prev_dir, char **history, int *index, int *command_count, char *input)
{
    if (command[0] == '|' || command[strlen(command) - 1] == '|')
    {
        fprintf(stderr, RED_TEXT "Invalid pipe\n" NORMAL_TEXT);

        return -1;
    }

    int pipe_commands[2];
    char input2[100];
    strcpy(input2, command);
    char commands[200][512];
    char *pipe_arguments[MAX_ARGUMENTS];
    int stdoutp = dup(1);
    int stdinp = dup(0);
    int pipe_comm_count = 0;

    char temp[1024];
    int temp_index = 0;
    for (int i = 0; i <= strlen(command); i++)
    {
        if (command[i] == '|')
        {
            temp[temp_index] = '\0';
            strcpy(commands[pipe_comm_count++], temp);
            temp_index = 0;
        }
        else if (command[i] == '\0')
        {
            temp[temp_index] = '\0';
            strcpy(commands[pipe_comm_count++], temp);
            temp_index = 0;
        }

        else
        {
            temp[temp_index++] = command[i];
        }
    }

    if (pipe(pipe_commands) < 0)
    {
        fprintf(stderr, RED_TEXT "Error in pipe" NORMAL_TEXT);

        return -1;
    }
    for (int i = 0; i < pipe_comm_count; i++)
    {
        if (strcmp(commands[i], "") == 0)
        {
            fprintf(stderr, RED_TEXT "Invalid use of pipe\n" NORMAL_TEXT);

            return -1;
        }
    }

    // The commented out code is to handle cases where there is nothing to the left or to the right of a pipe.
    //  The commented out code will make it act like the normal shell, and execute pipe till the empty string is found.

    int i = 0;
    // int f = 0;
    for (i = 0; i < -1 + pipe_comm_count; i++)
    {
        // if (strcmp(commands[i + 1], "") == 0)
        // {
        //     f = 1;
        //     break;
        // }

        if (pipe(pipe_commands) < 0)
        {
            fprintf(stderr, RED_TEXT);
            perror("Error in creating pipe");
            fprintf(stderr, NORMAL_TEXT);
            return -1;
        }
        dup2(pipe_commands[1], 1);

        close(pipe_commands[1]);
        parseCommand(commands[i], n, home_dir, prev_dir, history, index, command_count, input2);
        dup2(pipe_commands[0], 0);

        close(pipe_commands[0]);
    }
    // if (!f)
    // {
    dup2(stdoutp, 1);
    parseCommand(commands[pipe_comm_count - 1], n, home_dir, prev_dir, history, index, command_count, input2);
    dup2(stdinp, 0);
    // }
    // else
    // {
    //     dup2(stdoutp, 1);
    //     parseCommand(commands[i], n, home_dir, prev_dir, history, index, command_count, input2);
    //     dup2(stdinp, 0);
    // }

    return 0;
}