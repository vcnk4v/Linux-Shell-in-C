#include "headers.h"

int compare(const void *a, const void *b)
{
    back_proc aa = *(back_proc *)a;
    back_proc bb = *(back_proc *)b;
    if (aa.pid > bb.pid)
    {
        return 1;
    }
    else if (aa.pid < bb.pid)
    {
        return -1;
    }

    else
    {
        return 0;
    }
}

void checkBackgroundProcesses()
{
    int index = 0;

    while (index < n_backs)
    {
        int status;
        int pid = waitpid(backg_procs[index].pid, &status, WNOHANG);

        if (pid > 0)
        {
            printf("%s exited ", backg_procs[index].input);

            if (WIFEXITED(status))
            {
                printf("normally (%d)\n", backg_procs[index].pid);
            }
            else
            {
                printf("abnormally (%d)\n", backg_procs[index].pid);
            }

            for (int j = index; j < n_backs - 1; j++)
            {
                backg_procs[j] = backg_procs[j + 1];
            }

            n_backs--; // Decrement the count of background processes after removal
        }
        else if (pid == 0)
        {
            index++;
        }
        else if (pid < 0)
        {
            index++;
        }
    }
}