#ifndef __COMMANDSINPUT_H
#define __COMMANDSINPUT_H

int parseInput(char *input, char *home_dir, char *prev_dir, char **history, int *index, int *command_count, char *his_inp);
int parseCommand(char *command, int n, char *home_dir, char *prev_dir, char **history, int *index, int *command_count, char *input);

#endif