#include "headers.h"

void fg(int pid)
{
    if (kill(pid, 0) == 0)
    {

        kill(pid, SIGCONT);

        int status;
        tcsetpgrp(0, pid); // grp id 0
        waitpid(pid, &status, WUNTRACED);
        signal(SIGTTOU, SIG_IGN);
        tcsetpgrp(0, getpid()); // add back curr process
        signal(SIGTTOU, SIG_DFL);
        // printf("Process brought to foreground\n");
        fg_pid = pid;
    }
    else
    {
        printf("No such process found\n");
    }
}

void bg(int pid)
{
    if (kill(pid, SIGCONT) == 0)
    {
        printf("[%d] - Resumed in the background\n", pid);
    }
    else
    {
        printf("No such process found with PID: %d\n", pid);
    }
}