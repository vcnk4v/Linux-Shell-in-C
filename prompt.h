#ifndef __PROMPT_H
#define __PROMPT_H

void prompt(char *home_dir);
char *get_current_directory();
char *modify_dir_name(char *directory, const char *substring);

#endif