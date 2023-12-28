#ifndef __BACKGROUND_H
#define __BACKGROUND_H
#define MAX_BACK_PROCS 20

typedef struct
{
    int pid;
    char input[4096];
} back_proc;

void checkBackgroundProcesses();
int compare(const void *a, const void *b);
#endif