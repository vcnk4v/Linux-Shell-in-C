#ifndef __REDIRECTION_H
#define __REDIRECTION_H
int add_spaces(char *input);
int redirection_present(char **arguments, int arg_count);
int *handle_redirection(char **arguments, int arg_count);

#endif