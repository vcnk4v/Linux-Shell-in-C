#include "headers.h"
int n_backs = 0;
back_proc backg_procs[MAX_BACK_PROCS];
char foreground_prompt[1024];
int shell_pid, fg_pid;
char fg_name[1024];
// char *home_dir;
int main()
{
    fg_pid = -1;
    signal(SIGTSTP, control_z);
    signal(SIGINT, control_c);

    char *home_dir = get_current_directory();
    char prev_dir[4096] = "";

    char history_file[DIRECTORY_LENGTH];

    snprintf(history_file, sizeof(history_file), "%s/history.txt", home_dir);
    char *history[MAX_HISTORY_SIZE] = {0};
    int command_count = 0;
    int index = 0;
    read_history(history_file, &index, &command_count, history);

    while (1)
    {
        checkBackgroundProcesses();
        prompt(home_dir);
        char input[4096];
        if (fgets(input, sizeof(input), stdin) == NULL) // if ctrl+D is pressed
        {
            printf("\n");
            break;
        }

        input[strcspn(input, "\n")] = '\0';

        if (strcmp(input, "") == 0)
        {
            continue;
        }
        if (parseInput(input, home_dir, prev_dir, history, &index, &command_count, input) == 1)
        {
            exit(0);
        }
    }
    return 0;
}