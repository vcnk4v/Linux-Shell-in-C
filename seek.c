#include "headers.h"

int is_directory(const char *target_directory)
{
    struct stat statbuf;
    if (stat(target_directory, &statbuf) != 0)
    {
        return 0;
    }
    return S_ISDIR(statbuf.st_mode);
}
int print_matching_files(const char *path, const char *target, int directory, int files, int execute, char *abs_path)
{
    DIR *dir = opendir(path);
    if (dir == NULL)
    {
        fprintf(stderr, RED_TEXT "Couldn't open file\n" NORMAL_TEXT);
        perror("seek");
        return 0;
    }

    struct dirent *entry;
    int match_count = 0; // Track the number of matching files/directories

    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }

        char actual_abs_path[1024];
        snprintf(actual_abs_path, sizeof(actual_abs_path), "%s/%s", path, entry->d_name);

        struct stat file_stat;
        if (stat(actual_abs_path, &file_stat) != 0)
        {
            fprintf(stderr, RED_TEXT "Couldn't obtain file info\n" NORMAL_TEXT);
            perror("seek");
            return 0;
        }

        if (entry->d_type == DT_DIR)
        {
            if (!files)
            {
                if (strcmp(entry->d_name, target) == 0)
                {
                    printf("\033[34m%s/\033[0m\n", actual_abs_path);
                    strcpy(abs_path, actual_abs_path);
                    match_count++;
                }
            }
            match_count += print_matching_files(actual_abs_path, target, directory, files, execute, abs_path);
        }
        else
        {
            if (!directory)
            {
                // printf("%s ", entry->d_name);
                if (strcmp(entry->d_name, target) == 0)
                {
                    if (S_ISREG(file_stat.st_mode) && (file_stat.st_mode & S_IXUSR))
                    {
                        // Executable file
                        printf("\033[32m%s\033[0m\n", actual_abs_path);
                        strcpy(abs_path, actual_abs_path);
                    }
                    else
                    {
                        // Regular file
                        printf("%s\n", actual_abs_path);
                        strcpy(abs_path, actual_abs_path);
                    }
                    match_count++;
                }
            }
        }
    }

    closedir(dir);
    return match_count;
}
void seek_target(char *arguments[], int arg_count)
{
    int d_flag = 0, f_flag = 0, e_flag = 0;
    int search_index = 1;
    for (int i = 1; i < arg_count; i++)
    {
        if (arguments[i][0] == '-')
        {
            if (strcmp("-d", arguments[i]) == 0)
            {
                d_flag = 1;
                if (f_flag == 1)
                {
                    fprintf(stderr, RED_TEXT "ERROR: Invalid flags!\n" NORMAL_TEXT);
                }
            }
            else if (strcmp("-f", arguments[i]) == 0)
            {
                f_flag = 1;
                if (d_flag == 1)
                {
                    fprintf(stderr, RED_TEXT "ERROR: Invalid flags!\n" NORMAL_TEXT);
                }
            }
            else if (strcmp("-e", arguments[i]) == 0)
            {
                e_flag = 1;
            }
            else if (strcmp("-ef", arguments[i]) == 0 || strcmp("-fe", arguments[i]) == 0)
            {
                e_flag = 1;
                f_flag = 1;
            }
            else if (strcmp("-ed", arguments[i]) == 0 || strcmp("-de", arguments[i]) == 0)
            {
                e_flag = 1;
                d_flag = 1;
            }
            else
            {
                fprintf(stderr, RED_TEXT "ERROR: Invalid flags!\n" NORMAL_TEXT);
            }
        }
        else
        {
            search_index = i;
            break;
        }
    }
    char *search = arguments[search_index];
    char *target_directory = (search_index + 1 < arg_count) ? arguments[search_index + 1] : ".";
    char abs_path[DIRECTORY_LENGTH];
    int match_count = print_matching_files(target_directory, search, d_flag, f_flag, e_flag, abs_path);
    if (e_flag && match_count == 1)
    {
        if (!is_directory(abs_path))
        {
            FILE *file = fopen(abs_path, "r");
            if (file != NULL)
            {
                char content[100];
                while (fgets(content, sizeof(content), file))
                {
                    printf("%s", content);
                }
                fclose(file);
            }
            else
            {
                fprintf(stderr, RED_TEXT "Missing permissions for task\n" NORMAL_TEXT);
            }
        }
        else
        {
            chdir(abs_path);
        }
    }
    else if (match_count == 0)
    {
        printf("No match found!\n");
    }
}
