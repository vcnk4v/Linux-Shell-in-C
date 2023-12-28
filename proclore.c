#include "headers.h"

void process_info(int pid)
{

    char path_to_file[DIRECTORY_LENGTH];
    const char *plus_sign = "+";
    snprintf(path_to_file, sizeof(path_to_file), "/proc/%d/status", pid);
    FILE *file = fopen(path_to_file, "r");
    if (file == NULL)
    {
        perror("status file");
        return;
    }
    char status[20];
    while (fgets(path_to_file, sizeof(path_to_file), file) != NULL)
    {

        if (strncmp(path_to_file, "State:", 6) == 0)
        {
            sscanf(path_to_file, "State:\t%s", status);

            break;
        }
    }

    pid_t process_group = getpgid(pid);
    pid_t tc_pgid = tcgetpgrp(STDIN_FILENO);

    char line[256];
    while (fgets(line, sizeof(line), file))
    {
        if (strncmp(line, "VmSize:", 7) == 0)
        {
            break;
        }
    }
    fclose(file);

    char newString[20];
    if (process_group == tc_pgid)

    {
        snprintf(newString, sizeof(newString), "%s%s", status, plus_sign);
    }
    else
    {
        strcpy(newString, status);
    }

    printf("pid : %d\n", pid);
    printf("process status : %s\n", newString);
    printf("Process Group : %d\n", process_group);
    printf("Virtual memory : %s", 8 + line);
    snprintf(path_to_file, sizeof(path_to_file), "/proc/%d/exe", pid);
    ssize_t len = readlink(path_to_file, path_to_file, sizeof(path_to_file) - 1);
    if (len != -1)
    {
        path_to_file[len] = '\0';
        printf("executable path : %s\n", path_to_file);
    }
    else
    {
        fprintf(stderr, RED_TEXT);
        perror("Error reading executable path");
        fprintf(stderr, NORMAL_TEXT);
    }
}