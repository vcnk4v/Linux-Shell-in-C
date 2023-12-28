#include "headers.h"

int lexicographic(const void *a, const void *b)
{
    const char *x = *(const char **)a;
    const char *y = *(const char **)b;
    char first[1 + strlen(x)];
    char second[1 + strlen(y)];

    int index1 = 0, index2 = 0;
    if (x[0] == '.' && strcmp(x, ".") != 0 && strcmp(x, "..") != 0)
    {
        index1 = 1;
    }
    if (y[0] == '.' && strcmp(y, ".") != 0 && strcmp(y, "..") != 0)
    {
        index2 = 1;
    }
    for (int i = 0; i <= strlen(x); i++)
    {

        first[i] = tolower(x[index1++]);
    }
    for (int i = 0; i <= strlen(y); i++)
    {

        second[i] = tolower(y[index2++]);
    }

    return strcmp(first, second);
}

const char *permissions(mode_t mode)
{
    static char perms[11]; // 10 characters for permission string + 1 for null terminator

    perms[0] = S_ISDIR(mode) ? 'd' : '-';
    perms[1] = (mode & S_IRUSR) ? 'r' : '-';
    perms[2] = (mode & S_IWUSR) ? 'w' : '-';
    perms[3] = (mode & S_IXUSR) ? 'x' : '-';
    perms[4] = (mode & S_IRGRP) ? 'r' : '-';
    perms[5] = (mode & S_IWGRP) ? 'w' : '-';
    perms[6] = (mode & S_IXGRP) ? 'x' : '-';
    perms[7] = (mode & S_IROTH) ? 'r' : '-';
    perms[8] = (mode & S_IWOTH) ? 'w' : '-';
    perms[9] = (mode & S_IXOTH) ? 'x' : '-';
    perms[10] = '\0';

    return perms;
}
void peek(char *arguments[], int arg_count, char *home_dir, char *prev_dir)
{
    char *path = NULL;
    int a_flag = 0;
    int l_flag = 0;

    for (int i = 1; i < arg_count; i++)
    {
        if (arguments[i][0] == '-')
        {
            if (strcmp("-a", arguments[i]) == 0)
            {
                a_flag = 1;
            }
            else if (strcmp("-l", arguments[i]) == 0)
            {
                l_flag = 1;
            }
            else if (strcmp("-al", arguments[i]) == 0 || strcmp("-la", arguments[i]) == 0)
            {
                l_flag = 1;
                a_flag = 1;
            }
            else
            {
                fprintf(stderr, RED_TEXT "Invalid flag\n" NORMAL_TEXT);
                return;
            }
        }
        else
        {
            if (i == arg_count - 1)
            {
                path = arguments[i];
            }
        }
    }
    if (path == NULL)
    {
        path = get_current_directory();
    }

    if (strcmp(".", path) == 0)
    {
        path = get_current_directory();
    }
    else if (strcmp("~", path) == 0)
    {
        path = home_dir;
    }

    DIR *dir = opendir(path);
    if (dir == NULL)
    {
        perror("peek");
        return;
    }

    struct dirent *entry;
    char **names = NULL;
    int num_names = 0;

    while ((entry = readdir(dir)) != NULL)
    {
        if (!a_flag && entry->d_name[0] == '.')
        {
            continue; // Skip hidden files
        }

        names = realloc(names, (num_names + 1) * sizeof(char *));
        names[num_names++] = strdup(entry->d_name);
    }

    closedir(dir);

    qsort(names, num_names, sizeof(char *), lexicographic);

    for (int i = 0; i < num_names; i++)
    {
        char full_path[4096];
        snprintf(full_path, sizeof(full_path), "%s/%s", path, names[i]);

        struct stat file_stat;
        if (stat(full_path, &file_stat) == 0)
        {
            if (l_flag)
            {
                printf("%s ", permissions(file_stat.st_mode));
                printf("%2ld ", file_stat.st_nlink);

                struct passwd *pw = getpwuid(file_stat.st_uid);
                printf("%s ", pw ? pw->pw_name : "-");

                struct group *gr = getgrgid(file_stat.st_gid);
                printf("%s ", gr ? gr->gr_name : "-");

                printf("%7lld ", (long long)file_stat.st_size);

                struct tm *tm = localtime(&file_stat.st_mtime);
                char time_buf[80];
                strftime(time_buf, sizeof(time_buf), "%b %d %H:%M", tm);
                printf("%s ", time_buf);
            }

            if (S_ISDIR(file_stat.st_mode))
            {
                printf("\033[34m%s\033[0m\n", names[i]); // Blue for directories
            }
            else if (file_stat.st_mode & S_IXUSR || file_stat.st_mode & S_IXGRP || file_stat.st_mode & S_IXOTH)
            {
                printf("\033[32m%s\033[0m\n", names[i]); // Green for executables
            }
            else
            {
                printf("%s\n", names[i]); // White for other files
            }
        }

        free(names[i]);
    }

    free(names);
}