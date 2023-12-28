#include "headers.h"

void print_process_info(int pid, char *command, char *state)
{
    if (strncmp(state, "R", 1) == 0)
    {
        strcpy(state, "Running");
    }
    else
    {
        strcpy(state, "Stopped");
    }

    printf("%d : %s - %s\n", pid, command, state);
}

void activities_new(pid_t shell_pid)
{
    char *processfile = (char *)malloc(sizeof(char) * 1024);
    for (int i = 0; i < n_backs; i++)
    {
        char x[1024];
        char y[1024];
        char z[32];
        sprintf(processfile, "/proc/%d/status", backg_procs[i].pid);

        // sprintf(processfile, "/proc/%d/stat", backg_procs[i].pid);
        FILE *f = fopen(processfile, "r");
        if (f)
        {
            if (strcmp(z, "Z") == 0)
            {
                continue;
            }
            else
            {
                fscanf(f, "%s %s %s", x, y, z);
                printf("%d: %s -", backg_procs[i].pid, backg_procs[i].input);
                printf("%s\n", z);
                if (strcmp(z, "T") == 0)
                {
                    printf("Stopped\n");
                }

                else if (strcmp(z, "R") == 0 || strcmp(z, "S") == 0)
                {
                    printf("Running\n");
                }
            }
        }
    }
}