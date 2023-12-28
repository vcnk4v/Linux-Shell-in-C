#include "headers.h"

int other_comm(char **arguments, int arg_count, int background, int maf)
{
    time_t start, end;
    pid_t pid;
    pid = fork();
    fg_pid = pid;
    strcpy(fg_name, arguments[0]);
    if (pid < 0)
    {
        fprintf(stderr, RED_TEXT "Fork Error\n" NORMAL_TEXT);
        perror("Failed to create child");
        return -1;
    }
    else if (pid == 0)
    {
        setpgid(0, 0);
        if (execvp(arguments[0], arguments) < 0)
        {
            fprintf(stderr, RED_TEXT);
            perror("ERROR");
            fprintf(stderr, "%s is not a valid command.\n", arguments[0]);
            fprintf(stderr, NORMAL_TEXT);
            return -1;
        };
    }
    if (!background)
    {
        int status;
        start = time(NULL);

        waitpid(pid, &status, WUNTRACED);

        // end = clock();
        end = time(NULL);

        int execution_time = (int)(end - start);
        if (execution_time > 2)
        {
            snprintf(foreground_prompt, sizeof(foreground_prompt), "%s %.ds", arguments[0], execution_time);
        }
        fg_pid = -1;
    }
    else
    {

        printf("[%d] %d\n", n_backs + 1, pid);

        strcpy(backg_procs[n_backs].input, arguments[0]);
        backg_procs[n_backs].pid = pid;
        n_backs++;
        if (n_backs >= MAX_BACK_PROCS)
        {
            printf("Max background processes reached.\n");
        }
        qsort(backg_procs, n_backs, sizeof(back_proc), compare);
    }
}

int parseCommand(char *command, int n, char *home_dir, char *prev_dir, char **history, int *index, int *command_count, char *input)
{
    shell_pid = getpid();
    int background = 0;
    char new_command[4096];

    // char *arguments[MAX_ARGUMENTS];
    char **arguments = (char **)malloc(sizeof(char *) * MAX_ARGUMENTS);
    for (int i = 0; i < 20; i++)
    {
        arguments[i] = (char *)malloc(sizeof(char) * 512);
    }

    int arg_count = 0;
    strcpy(foreground_prompt, "");
    int multiple_ampersand_flag = 0;

    // Remove leading spaces and tabs

    while (isspace(*command))
    {
        command++;
    }

    // Check for '&' at the end of the command
    char *ampersand = strchr(command, '&');

    if (ampersand != NULL && ampersand - command == strlen(command) - 1)
    {
        background = 1;
        *ampersand = '\0';
    }
    else if (ampersand != NULL)
    {

        background = 1;
        multiple_ampersand_flag = 1;
        int j = 0;
        for (int i = ampersand - command + 1; i < strlen(command); i++)
        {
            new_command[j++] = command[i];
        }
        new_command[j] = '\0';

        *ampersand = '\0';
    }
    char command_copy[4096];
    strcpy(command_copy, command);
    // Tokenize command into arguments using spaces and tabs as delimiters
    int single_quot_flag = 0;

    for (int i = 0; i < strlen(command); i++)
    {
        int word_index = 0;

        if (command[i] == '\'')
        {
            i++;
            while (i < strlen(command) && command[i] != '\'')
            {
                arguments[arg_count][word_index++] = command[i];
                i++;
            }
            if (command[i] != '\'')
            {
                fprintf(stderr, RED_TEXT "Single quotes not closed\n" NORMAL_TEXT);
                return -1;
            }

            i++;
            arguments[arg_count][word_index++] = '\0';

            arg_count++;
        }
        else if (command[i] == '"')
        {
            i++;
            while (i < strlen(command) && command[i] != '"')
            {
                arguments[arg_count][word_index++] = command[i];
                i++;
            }
            if (command[i] != '"')
            {
                fprintf(stderr, RED_TEXT "Double quotes not closed\n" NORMAL_TEXT);
                return -1;
            }

            i++;
            arguments[arg_count][word_index++] = '\0';

            arg_count++;
        }

        else
        {
            while (i < strlen(command) && command[i] != ' ' && command[i] != '\t')
            {
                arguments[arg_count][word_index++] = command[i];
                i++;
            }
            arguments[arg_count][word_index++] = '\0';

            if (i != 0 && command[i - 1] != ' ' && command[i - 1] != '\t')
            {
                arg_count++;
            }
        }
    }
    int *stds;
    arguments[arg_count] = NULL;
    if (arg_count == 0)
    {
        return 0;
    }

    int redir_flag = redirection_present(arguments, arg_count);

    if (redir_flag == 1)
    {
        stds = handle_redirection(arguments, arg_count);
    }
    else if (redir_flag == -1)
    {
        return -1;
    }
    if (stds == NULL && redir_flag == 1)
    {
        return -1;
    }

    if (strcmp(arguments[0], "exit") == 0)
    {
        return -2;
    }

    else if (strcmp(arguments[0], "warp") == 0)
    {
        warp(arguments, arg_count, home_dir, prev_dir);
    }
    else if (strcmp(arguments[0], "peek") == 0)
    {
        peek(arguments, arg_count, home_dir, prev_dir);
    }
    else if (strcmp(arguments[0], "pastevents") == 0)
    {
        char history_file[1024];
        snprintf(history_file, sizeof(history_file), "%s/history.txt", home_dir);

        if (arg_count == 1)
        {
            print_history(history_file, history);
        }
        else if (strcmp(arguments[1], "purge") == 0)
        {
            purge(history_file, index, command_count, history);
        }
        else if (strcmp(arguments[1], "execute") == 0)
        {

            int exec_index = atoi(arguments[2]);
            pastevents_execute(history_file, exec_index, home_dir, prev_dir, history, index, command_count, input);
        }

        else
        {
            fprintf(stderr, RED_TEXT "ERROR: Invalid argument\n" NORMAL_TEXT);
        }
    }
    else if (strcmp("proclore", arguments[0]) == 0)
    {
        if (arg_count == 1)
        {
            process_info(shell_pid);
        }
        else if (arg_count == 2)
        {
            int pid = atoi(arguments[1]);
            process_info(pid);
        }
        else
        {
            fprintf(stderr, RED_TEXT "ERROR: Invalid proclore command\n" NORMAL_TEXT);

            return -1;
        }
    }
    else if (strcmp("seek", arguments[0]) == 0)
    {
        seek_target(arguments, arg_count);
    }
    else if (strcmp("activities", arguments[0]) == 0)
    {
        activities_new(shell_pid);
    }
    else if (strcmp("iMan", arguments[0]) == 0)
    {
        fetch_man_page(arguments[1]);
    }
    else if (strcmp("fg", arguments[0]) == 0)
    {

        if (arg_count != 2)
        {
            fprintf(stderr, RED_TEXT "ERROR: Invalid arguments to fg.\n" NORMAL_TEXT);
            return -1;
        }
        else
        {
            int proccess_id = atoi(arguments[1]);
            fg(proccess_id);
        }
    }
    else if (strcmp("bg", arguments[0]) == 0)
    {
        if (arg_count != 2)
        {
            fprintf(stderr, RED_TEXT "ERROR: Invalid arguments to fg.\n" NORMAL_TEXT);
            return -1;
        }
        else
        {
            int proccess_id = atoi(arguments[1]);
            bg(proccess_id);
        }
    }

    else if (strcmp("ping", arguments[0]) == 0)
    {
        if (arg_count != 3)
        {
            fprintf(stderr, RED_TEXT "ERROR: Invalid arguments to ping.\n" NORMAL_TEXT);
            return -1;
        }
        else
        {
            int proccess_id = atoi(arguments[1]);
            int sig_num = atoi(arguments[2]);
            ping_signal(proccess_id, sig_num);
        }
    }
    else if (strcmp("neonate", arguments[0]) == 0)
    {
        if (arg_count != 3)
        {
            fprintf(stderr, RED_TEXT "ERROR: Invalid arguments to neonate.\n" NORMAL_TEXT);
            return -1;
        }
        else if (strcmp(arguments[1], "-n") != 0)
        {
            fprintf(stderr, RED_TEXT "ERROR: Invalid arguments to fg.\n" NORMAL_TEXT);
            return -1;
        }
        else
        {
            int n = atoi(arguments[2]);
            neonate(n);
        }
    }

    else
    {
        other_comm(arguments, arg_count, background, multiple_ampersand_flag);
    };

    if (multiple_ampersand_flag == 1)
    {
        parseCommand(new_command, n + 1, home_dir, prev_dir, history, index, command_count, input);
    }
    for (int i = 0; i < MAX_ARGUMENTS; i++)
    {
        free(arguments[i]);
    }
    free(arguments);

    if (redir_flag)
    {
        dup2(stds[0], 0);
        dup2(stds[1], 1);
    }

    return 0;
}

int parseInput(char *input, char *home_dir, char *prev_dir, char **history, int *index, int *command_count, char *his_inp)
{
    if (strlen(input) == 1)
    {
        return 0;
    }
    char input2[4096];
    strcpy(input2, input);
    while (isspace(*input))
    {
        input++;
    }

    int num_commands = 0;
    char commands[200][200];

    char history_file[DIRECTORY_LENGTH];
    snprintf(history_file, sizeof(history_file), "%s/history.txt", home_dir);

    add_to_history(his_inp, history, index, command_count);
    save_history(history, *command_count, history_file, *index);
    if (add_spaces(input) == -1)
    {
        return -1;
    }

    char *token = strtok(input, ";");

    while (token != NULL)
    {
        strcpy(commands[num_commands++], token);
        token = strtok(NULL, ";");
    }
    for (int i = 0; i < num_commands; i++)
    {
        if (checkpiping(commands[i]))
        {
            if (piping(commands[i], i, home_dir, prev_dir, history, index, command_count, input2) < 0)
            {
                return -1;
            }
        }
        else
        {
            int x = parseCommand(commands[i], i, home_dir, prev_dir, history, index, command_count, input2);
            if (x == -2)
            {
                exit(0);
            }
        }
    }
    return 0;
}
