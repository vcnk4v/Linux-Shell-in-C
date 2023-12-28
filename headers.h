#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/wait.h>
#include <pwd.h>
#include <sys/stat.h>
#include <string.h>
#include <dirent.h>
#include <time.h>
#include <fcntl.h>
#include <grp.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <termios.h>
#include <signal.h>

#include "background.h"
#include "seek.h"
#include "prompt.h"
#include "peek.h"
#include "warp.h"
#include "pastevents.h"
#include "commandsinput.h"
#include "proclore.h"

#include "redirection.h"
#include "pipe.h"
#include "activities.h"
#include "iman.h"
#include "neonate.h"
#include "ping.h"
#include "fgbg.h"

#define MAX_HISTORY_SIZE 15
#define MAX_ARGUMENTS 20
#define DIRECTORY_LENGTH 1024
#define HOST_NAME_LENGTH 256
#define RED_TEXT "\033[31m"
#define NORMAL_TEXT "\033[0m"

extern int shell_pid;
extern int n_backs;
extern back_proc backg_procs[MAX_BACK_PROCS];
extern char foreground_prompt[1024];
extern int fg_pid;
extern char fg_name[1024];
// extern *home_dir;
