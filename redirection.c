#include "headers.h"
int add_spaces(char *input)
{
    int count = 0;
    int len = strlen(input);
    for (int i = 0; i < len; i++)
    {
        if (input[i] == '<' || input[i] == '>')
        {
            count++;
        }
    }
    char newinput[len + count];
    if (input[0] == '>' || input[0] == '<' || (input[0] == '>' && input[1] == '>') || input[len - 1] == '<' || input[len - 1] == '>')
    {
        fprintf(stderr, RED_TEXT "Invalid redirection\n" NORMAL_TEXT);
        return -1;
    }

    int index = 0;
    for (int i = 0; i < len; i++)
    {
        if (input[i] == '<')
        {
            if (input[i - 1] != ' ')
            {
                newinput[index++] = ' ';
            }

            newinput[index++] = '<';
            if (input[i + 1] != ' ')
            {
                newinput[index++] = ' ';
            }
        }
        else if (i != len - 1 && input[i] == '>' && input[i + 1] == '>')
        {
            if (input[i - 1] != ' ')
            {
                newinput[index++] = ' ';
            }
            newinput[index++] = '>';
            newinput[index++] = '>';
            if (input[i + 2] != ' ')
            {
                newinput[index++] = ' ';
            }
            i++;
        }
        else if (input[i] == '>')
        {
            if (input[i - 1] != ' ')
            {
                newinput[index++] = ' ';
            }
            newinput[index++] = '>';
            if (input[i + 1] != ' ')
            {
                newinput[index++] = ' ';
            }
        }
        else
        {
            newinput[index++] = input[i];
        }
    }
    newinput[index] = '\0';
    strcpy(input, newinput);
}
int redirection_present(char **arguments, int arg_count)
{
    for (int i = 0; i < arg_count; i++)
    {
        if ((i == 0 || i == arg_count) && (strcmp(arguments[i], ">>") == 0 || strcmp(arguments[i], ">") == 0 || strcmp(arguments[i], "<") == 0))
        {
            fprintf(stderr, RED_TEXT "Invalid redirection\n" NORMAL_TEXT);
            return -1;
        }

        if (strcmp(arguments[i], ">") == 0)
        {
            return 1;
        }
        else if (strcmp(arguments[i], "<") == 0)
        {
            return 1;
        }
        else if (strcmp(arguments[i], ">>") == 0)
        {
            return 1;
        }
    }
    return 0;
}
int *handle_redirection(char **arguments, int arg_count)
{
    int writeflag = 0;
    int appendflag = 0;
    int inputt = 0;
    int inputfile, outputfile;
    char output[DIRECTORY_LENGTH];
    char input[DIRECTORY_LENGTH];
    int std_out = dup(1);
    int std_in = dup(0);

    for (int i = 0; i < arg_count - 1; i++)
    {
        if (arguments[i] != NULL && strcmp(arguments[i], ">>") == 0)
        {
            appendflag = 1;
            arguments[i] = NULL;
            strcpy(output, arguments[i + 1]);
        }
        if (arguments[i] != NULL && strcmp(arguments[i], "<") == 0)
        {
            inputt = 1;
            arguments[i] = NULL;
            strcpy(input, arguments[i + 1]);
        }
        if (arguments[i] != NULL && strcmp(arguments[i], ">") == 0)
        {
            writeflag = 1;
            arguments[i] = NULL;
            strcpy(output, arguments[i + 1]);
        }
    }

    if (appendflag != 0 && writeflag != 0)
    {
        fprintf(stderr, RED_TEXT "Cannot both append and write." NORMAL_TEXT);
        return NULL;
    }

    if (writeflag)
    {
        // printf("x");

        outputfile = open(output, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (outputfile < 0)
        {
            fprintf(stderr, RED_TEXT);
            perror("Error in opeing output file ");
            fprintf(stderr, NORMAL_TEXT);
            return NULL;
        }
        dup2(outputfile, 1);
    }

    else if (appendflag != 0)
    {

        outputfile = open(output, O_WRONLY | O_CREAT | O_APPEND, 0644);
        if (outputfile < 0)
        {
            fprintf(stderr, RED_TEXT);
            perror("Error in opeing output file ");
            fprintf(stderr, NORMAL_TEXT);
            return NULL;
        }
        dup2(outputfile, 1);
    }
    if (inputt)
    {

        inputfile = open(input, O_RDONLY, 0644);
        if (inputfile < 0)
        {
            fprintf(stderr, RED_TEXT);
            perror("Error in opeing input file ");
            fprintf(stderr, NORMAL_TEXT);
            return NULL;
        }
        dup2(inputfile, 0);
    }
    int *stds = (int *)malloc(sizeof(int) * 2);
    stds[0] = std_in;
    stds[1] = std_out;
    return stds;
}