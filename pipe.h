#ifndef __PIPE_H
#define __PIPE_H
int checkpiping(char *command);
int piping(char *command, int n, char *home_dir, char *prev_dir, char **history, int *index, int *command_count, char *input);
#endif