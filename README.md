# Description

## Instructions to compile and run

run `make` to compile and then run `./a.out` to start the shell.

## Functions in each c file

### main.c

1. Main function
2. Starts the shell and contains the infinte loop for the shell

### prompt.c

1. Prints the prompt for the shell in this format : <Username@SystemName:directory>

### commandsinput.c

1. Tokenises the input according to ";" and parses each command allowing to give multiple commands at the same time. If piping is detected, it handles piping through `piping` function.
2. The function parseCommand tokenises each command according to "\t" or " ".
3. Checks for redirection and handles it accordingly.
4. Forks the parent process to create child to process the command.
5. If & symbol is found, runs command in background.

### warp.c

1. Implementation of warp command (similar to cd).

### peek.c

1. Implementation of peek command (similar to ls).

### pastevents.c

1. Implementation of pastevents command (similar to history).
2. Contains the `pastevents_purge`, `pastevents` and `pastevents_execute` functions.

### background.c

1. Checks in the list of background processes if a background process is over.
2. Prints process name along with pid when background process ends.

### proclore.c

1. Implementation of proclore command.
2. Prints info related to a process :

   - pid
   - Process Status (R/R+/S/S+/Z)
   - Process group
   - Virtual Memory
   - Executable path of process

### seek.c

1. Implementation of seek command.

### activities.c

1. Implementation of activities command. (similar to jobs).

### ping.c

1. Implementation of the ping command.
2. Also contains functions for handling ctrl+C, ctrl+Z, and ctrl+D.

### fgbg.c

1. Implementations of fg and bg commands

### iman.c

1. Implementation of iMan command. (similar to man command)

### neonate.c

1. Implementation of neonate command.

### pipe.c

1. Function `checkpiping` to check for piping and `piping` to handle piping.

### redirection.c

1. Functions `add_spaces` to help in tokenising, `redirection_present` to check for redirection, and `handle_redirection` to handle redirection.

# Assumptions

- Colors:

  - All errors are printed in red.
  - All directories are printed in blue.
  - All executables are printed in green.
  - All files are printed in white.

- Pastevents:

  - Commands with errors are also stored in history (similar to history).
  - If a command like `sleep 1; pastevents execute 1` is given it will execute the command given before this input, not `sleep 1`.
  - If `pastevents execute` is called, it will be stored in history as the command it executes, i.e. the command to be executed will replace the pastevents exceute command in the input while writing into history.
  - The `pastevents` command prints all commands (maximum 15) stored in history in the order of execution, i.e. the latest command will be printed last.
  - `pastevents execute n` will execute the last nth command.

- Background:

  - Background processes for commands implemented are not handled, only handled for existing shell commands.
  - Maximum background processes that can run is 20.
  - Whenever the user presses enter on prompt, all background processes that have been exited are displayed with name, pid and whether they exited normally or abnormally.

- Warp:

  - Calling warp with `-` for the first time when shell is invoked will get the error "OLDPWD not set"

- Seek:

  - When seek with `-e` flag is called, it will print "Missing permissions for this task" if user does not have permission for the exe file.

- Activities:

  - Will print processes with state T as stopped, all others as running.

- iMan:

  - Internet connection is required.
  - Will also output some html formatting like `<a>, <strong>, <h1>, <centre>` etc.
  - The content of the website is printed from where NAME starts to the end of the body, i.e. `NAME...</body>`.

- Direct keyboard input signals:

  - ctrl+C / ctrl+Z are implemented for shell-built in commands (for which execvp is used) only, not for custom made commands.
  - These do not stop neonate.

- Piping

  - Will give error “Invalid use of pipe”, if there is nothing to the left or to the right of a pipe.
  - I have commented out the code which will handle cases like `echo hi || wc` the way it is handled in bash, i.e. execute commands until `||`.
  - It supports any number of pipes (but not multiple inputs and outputs from I/O redirection).
  - For pastevents with piping, if commands like `pastevents execute n | cmd` are given, the output of `pastevents execute 1` is piped into cmd, but only the command in `pastevents execute n` is stored in history. However if the command is of the format `cmd | pastevents execute n`, then the entire command is stored in history. (The command that is stored is the one until `pastevents execute n`)

- I/O redirection
  - For pastevents if commands like `pastevents execute n > file` are given, the output of `pastevents execute 1` is redirected into file, but only the command in `pastevents execute n` is stored in history.
  - For commands like `echo hello | wc < a.txt`, redirection is prioritized over pipe.
