#include "headers.h"

void ping_signal(int pid, int sig_num)
{
    if (pid <= 0 || sig_num <= 0)
    {
        fprintf(stderr, RED_TEXT "Invalid PID or signal number\n" NORMAL_TEXT);
        return;
    }
    int sts = sig_num % 32;
    if (kill(pid, sig_num) < 0)
    {
        printf("No process found with PID: %d\n", pid);
    }
    else
    {
        printf("Sent signal %d to process with pid %d\n", sig_num, pid);
    }
}

void control_z(int sig)
{
    if (fg_pid > 0)
    {
        printf("Sending SIGTSTP to PID: %d\n", fg_pid);
        strcpy(backg_procs[n_backs].input, fg_name);
        backg_procs[n_backs++].pid = fg_pid;

        if (kill(fg_pid, SIGTSTP) == -1)
        {
            fprintf(stderr, RED_TEXT);
            perror("Failed to send SIGTSTP");
            fprintf(stderr, RED_TEXT);
        }
        setpgid(fg_pid, 0);

        fg_pid = -1;
        fg_name[0] = '\0';
    }
}

void control_c(int sig)
{
    if (fg_pid > 0)
    {
        if (kill(fg_pid, SIGINT))
        {
            fprintf(stderr, RED_TEXT);
            perror("Failed to send SIGINT");
            fprintf(stderr, RED_TEXT);
        }
        fg_pid = -1;
        printf("\n");
        return;
    }
    // prompt(home_dir);
    return;
}
