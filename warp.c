#include "headers.h"
void warp(char *arguments[], int arg_count, char *home_dir, char *prev_dir)
{
    char *current = get_current_directory();

    if (arg_count > 1)
    {
        for (int i = 1; i < arg_count; i++)
        {
            char *path = arguments[i];

            if (strcmp(path, "..") == 0)
            {
                chdir("..");
                strcpy(prev_dir, current);
            }
            else if (strcmp(path, "~") == 0)
            {
                chdir(home_dir);
                strcpy(prev_dir, current);
            }
            else if (strcmp(path, "-") == 0)
            {
                // previous
                if (strcmp(prev_dir, "") == 0)
                {

                    fprintf(stderr, RED_TEXT "ERROR: OLDPWD not set\n" NORMAL_TEXT); // perror
                    return;
                }

                chdir(prev_dir);
                strcpy(prev_dir, current);
            }
            else
            {
                // Change to the specified directory
                if (chdir(path) != 0)
                {
                    fprintf(stderr, RED_TEXT);
                    perror("warp");
                    fprintf(stderr, NORMAL_TEXT);
                }
                strcpy(prev_dir, current);
            }

            // Print the current working directory
            char *cwd = get_current_directory();
            if (cwd != NULL)
            {
                printf("%s\n", cwd);
            }
        }
    }
    else
    {

        // Warp to the home directory
        chdir(home_dir);
        strcpy(prev_dir, current);
    }
}