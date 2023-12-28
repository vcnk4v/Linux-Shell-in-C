#include "headers.h"

void replacePastEvents(char *str, const char *newSubstr)
{
    char *pos = str;

    if ((pos = strstr(pos, "pastevents")) != NULL)
    {
        char *executePos = strstr(pos, "execute");
        if (executePos != NULL)
        {
            char *semicolonPos = strchr(executePos, ';');
            if (semicolonPos == NULL)
            {
                semicolonPos = strchr(executePos, '\0');
            }

            int replaceLen = semicolonPos - pos + 1;

            strncpy(pos, newSubstr, strlen(newSubstr));
            memmove(pos + strlen(newSubstr), semicolonPos, strlen(semicolonPos) + 1);
        }
    }
}

void read_history(char *history_file, int *index, int *command_count, char *history[])
{
    FILE *file = fopen(history_file, "r");
    if (file == NULL)
    {
        FILE *file = fopen(history_file, "w");
        if (file == NULL)
        {
            fprintf(stderr, RED_TEXT "ERROR: Couldn't open file\n" NORMAL_TEXT);
            perror("pastevents");
        }

        fprintf(file, "%s", "0");
        return;
    }
    char buffer1[2];
    fread(buffer1, 1, sizeof(buffer1), file);

    *index = atoi(buffer1);

    char buffer[4096];
    while (fgets(buffer, sizeof(buffer), file) != NULL)
    {
        buffer[strcspn(buffer, "\n")] = '\0';
        history[(*command_count) % MAX_HISTORY_SIZE] = strdup(buffer);
        *command_count = *command_count + 1;
    }

    fclose(file);
}

void save_history(char *history[], int command_count, char *history_file, int index)
{
    FILE *file = fopen(history_file, "w");
    if (file == NULL)
    {
        fprintf(stderr, RED_TEXT);
        perror("pastevents");
        fprintf(stderr, NORMAL_TEXT);
        return;
    }
    if (index < 10)
    {
        char str[3];
        sprintf(str, "%d", index);
        fprintf(file, "%s ", str);
    }
    else
    {
        char str[3];
        sprintf(str, "%d", index);
        fprintf(file, "%s", str);
    }

    for (int i = 0; i < command_count; i++)
    {
        fprintf(file, "%s\n", history[i]);
    }

    fclose(file);
}

int add_to_history(char *input, char *history[], int *index, int *command_count)
{
    if (strstr(input, "pastevents") != NULL)
    {
        return 0;
    }

    if (*command_count > 0 && strcmp(input, history[(*index - 1 + MAX_HISTORY_SIZE) % MAX_HISTORY_SIZE]) == 0)
    {
        return 0; // Skip duplicate command
    }

    if (*command_count >= MAX_HISTORY_SIZE)
    {
        free((char *)history[*index]);
    }
    else
    {
        (*command_count)++;
    }

    history[*index] = strdup(input);
    *index = (*index + 1) % MAX_HISTORY_SIZE;

    return 1; // Command added to history
}

void purge(char *history_file, int *index, int *command_count, char **history)
{
    for (int i = 0; i < MAX_HISTORY_SIZE; i++)
    {
        history[i] = NULL;
    }

    remove(history_file);
    *index = 0;
    *command_count = 0;
}

void pastevents_execute(char *history_file, int execution_index, char *home_dir, char *prev_dir, char **history, int *index, int *command_count, char *input)
{

    if (execution_index > 15 || execution_index > *command_count)
    {
        fprintf(stderr, RED_TEXT "SYNTAX ERROR: Invalid index\n" NORMAL_TEXT);
        return;
    }
    FILE *file = fopen(history_file, "r");
    char buffer1[2];
    fread(buffer1, 1, sizeof(buffer1), file);

    int i = atoi(buffer1);

    i = (i - execution_index + MAX_HISTORY_SIZE) % MAX_HISTORY_SIZE;
    char buffer[4096];
    for (int j = 0; j < i + 1; j++)
    {
        fgets(buffer, sizeof(buffer), file);
    }

    buffer[strcspn(buffer, "\n")] = '\0';
    replacePastEvents(input, buffer);
    parseInput(buffer, home_dir, prev_dir, history, index, command_count, input);
    fclose(file);
}

void print_history(char *history_file, char **history)
{
    char buffer[4096];
    FILE *file = fopen(history_file, "r");
    char buffer1[2];
    fread(buffer1, 1, sizeof(buffer1), file);

    int i = atoi(buffer1);

    for (int j = i; j < 15; j++)
    {
        if (history[j] == NULL)
        {
            break;
        }

        printf("%s\n", history[j]);
    }

    for (int j = 0; j < i; j++)
    {

        if (history[j] == NULL)
        {
            break;
        }
        printf("%s\n", history[j]);
    }

    fclose(file);
}