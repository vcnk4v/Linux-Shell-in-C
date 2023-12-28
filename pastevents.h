#ifndef __PASTEVENTS_H
#define __PASTEVENTS_H
void save_history(char *history[], int command_count, char *history_file, int index);
int add_to_history(char *input, char *history[], int *index, int *command_count);
void read_history(char *history_file, int *index, int *command_count, char *history[]);
void print_history(char *history_file, char **history);
void purge(char *history_file, int *index, int *command_count, char **history);
void pastevents_execute(char *history_file, int execution_index, char *home_dir, char *prev_dir, char **history, int *index, int *command_count, char *input);

#endif