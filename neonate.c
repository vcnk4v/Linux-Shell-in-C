#include "headers.h"

struct termios orig_termios;
int tm;

void die(const char *s)
{
    fprintf(stderr, RED_TEXT);
    perror(s);
    fprintf(stderr, NORMAL_TEXT);
    exit(1);
}
void disableRawMode()
{
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
        die("tcsetattr");
}
/**
 * Enable row mode for the terminal
 * The ECHO feature causes each key you type to be printed to the terminal, so you can see what you’re typing.
 * Terminal attributes can be read into a termios struct by tcgetattr().
 * After modifying them, you can then apply them to the terminal using tcsetattr().
 * The TCSAFLUSH argument specifies when to apply the change: in this case, it waits for all pending output to be written to the terminal, and also discards any input that hasn’t been read.
 * The c_lflag field is for “local flags”
 */
void enableRawMode()
{
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1)
        die("tcgetattr");
    atexit(disableRawMode);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
        die("tcsetattr");
}
void printPID(int signum)
{
    // Get and print the Process-ID
    // proc/sys/kernel
    char path_to_file[1024];

    snprintf(path_to_file, sizeof(path_to_file), "/proc/sys/kernel/ns_last_pid");
    FILE *file = fopen(path_to_file, "r");
    if (file == NULL)
    {
        fprintf(stderr, RED_TEXT);
        perror("Last pid");
        fprintf(stderr, NORMAL_TEXT);
    }

    char buff[16];
    fgets(buff, sizeof(buff), file);
    printf("%s", buff);
    // Set the alarm again for periodic printing
    alarm(tm);
}

void neonate(int n)
{

    enableRawMode();
    tm = n;

    // Set up a timer to periodically print the PID
    signal(SIGALRM, printPID);
    alarm(tm);
    // Wait for 'x' key press
    while (1)
    {
        char ch;
        if (read(STDIN_FILENO, &ch, 1) == 1 && ch == 'x')
        {
            alarm(0);
            break;
        }
    }

    disableRawMode();
}
