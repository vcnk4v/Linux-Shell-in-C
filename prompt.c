#include "headers.h"
char *get_current_directory()
{
    char *cwd = (char *)malloc(DIRECTORY_LENGTH);
    if (getcwd(cwd, DIRECTORY_LENGTH) != NULL)
    {
        return cwd;
    }
    else
    {
        fprintf(stderr, RED_TEXT "Error: Failed to get file information for %s\n", cwd);
        perror("getcwd");
        return NULL;
    }
}

char *modify_dir_name(char *directory, const char *substring)
{
    char *result = strstr(directory, substring);

    if (result != NULL)
    {
        // Calculate the position of the substring in the directory
        int position = result - directory;

        // Calculate the length of the new directory string
        size_t newLength = strlen(directory) - strlen(substring) + 2; // +2 for "~/"

        // Allocate memory for the new directory string
        char *newDirectory = (char *)malloc(newLength);
        if (newDirectory == NULL)
        {
            perror("malloc");
            return NULL;
        }

        // Copy the first part of the directory to the new string
        strncpy(newDirectory, directory, position);

        // Append "~/"
        newDirectory[position] = '~';

        // Copy the rest of the directory after the substring
        strcpy(newDirectory + position + 1, result + strlen(substring));

        // Return the modified directory name
        return newDirectory;
    }
    else
    {
        // Substring not found, return the original directory name
        return strdup(directory);
    }
}

void prompt(char *home_dir)
{
    struct passwd *pw;
    uid_t uid = geteuid();

    pw = getpwuid(uid);
    if (pw == NULL)
    {
        perror("getpwuid");
        return;
    }

    // Obtain username
    char *username = pw->pw_name;

    // Obtain system name
    char sysname[HOST_NAME_LENGTH];
    gethostname(sysname, sizeof(sysname));

    char *cwd = get_current_directory();
    char *directory_name = modify_dir_name(cwd, home_dir);

    // print prompt
    printf("<\033[32m%s@%s\033[0m:\033[34m%s\033[0m %s>", username, sysname, directory_name, foreground_prompt);

    free(cwd);
    free(directory_name);
}
