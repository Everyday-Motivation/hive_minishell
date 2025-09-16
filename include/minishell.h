/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaeklee <jaeklee@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 12:44:00 by timurray          #+#    #+#             */
/*   Updated: 2025/09/16 13:45:20 by jaeklee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <dirent.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <string.h>
# include <sys/ioctl.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <termcap.h>
# include <termios.h>

// Builtin functions
int		builtin_pwd(void);
int		builtin_cd(char **args);

// Signal
void	sigint_handler(int signal);
void	set_sig_action(void);


typedef struct s_arena {
    char *block; 
    size_t size;       
    size_t capacity;
} t_arena;

//jaekwang 
//arena

int arena_init(t_arena *arena);
void arena_free(t_arena *arena);
char *arena_strdup(t_arena *arena, const char *s);

//input
int get_input(t_arena *arena, char **input);
int check_input(t_arena *arena,char **input);
//get env
int init_env(t_vec *env, char **envp);
int copy_env(t_vec *env, char **envp);
int pwd_exists(t_vec *env);
int add_pwd(t_vec *env);
int increment_shlvl(t_vec *env);


#endif

/*
<readline/readline.h> / <readline/history.h>
readline		// Shows a prompt,edit a line; returns the typed string.
rl_clear_history// Wipes the saved input history from memory.
rl_on_new_line	// Tells rl you're starting a fresh line(after printing).
rl_replace_line // Replaces the current input buffer with your text.
rl_redisplay    // Redraws the input line on screen.
add_history     // Saves a line to the history (so ↑ can recall it).

<stdio.h>
printf          // Prints formatted text to stdout.
perror          // Prints your message + the latest errno string to stderr.

<stdlib.h>
malloc          // Allocates a chunk of memory; you free it later.
free            // Releases memory you got from malloc/realloc/calloc.
exit            // Ends the process immediately with a status code.
getenv          // Gets the value of an environment variable by name.

<unistd.h>
write           // Writes bytes to a file descriptor.
access         	// Checks if a file is reachable/readable/writable/exe.
open            // Opens a file and returns a file descriptor.
read            // Reads bytes from a file descriptor.
close           // Closes a file descriptor.
fork            // Clones the current process (parent + child).
execve          // Replaces the current process image with a new program.
dup             // Duplicates a file descriptor to the lowest free number.
dup2            // Duplicates a fd into a specific target number.
pipe            // Creates a unidirectional data channel (read end
	+ write end).
isatty          // Checks if a fd is a terminal.
ttyname         // Returns the path of the terminal for a fd.
ttyslot         // Returns the index/slot of the current terminal.
getcwd          // Gets the current working directory path.
chdir           // Changes the current working directory.
unlink          // Deletes a filesystem name (removes a file link).

<sys/wait.h>
wait            // Waits for any child to finish; returns its status.
waitpid         // Waits for a specific child (or with options).
wait3           // Like wait, also returns resource usage stats.
wait4           // Like waitpid, also returns resource usage stats.

<signal.h>
signal          // Sets a simple handler for a signal.
sigaction       // Sets a robust handler + flags for a signal.
sigemptyset     // Clears a signal set to “no signals”.
sigaddset       // Adds one signal to a signal set.
kill            // Sends a signal to a process (or group).

<sys/stat.h>
stat            // Gets file info by pathname (size, mode, times, etc.).
lstat           // Like stat, but doesn’t follow symlinks.
fstat           // Like stat, but by file descriptor.

<dirent.h>
opendir         // Opens a directory stream for reading entries.
readdir         // Reads the next entry (file name, type) in the directory.
closedir        // Closes the directory stream.

<string.h>
strerror        // Returns a human-readable string for an errno value.

<sys/ioctl.h>
ioctl           // Issues device/terminal control commands on a fd.

<termios.h>
tcsetattr       // Sets terminal I/O settings (canonical mode, echo, etc.).
tcgetattr       // Gets current terminal I/O settings.

<termcap.h>
tgetent         // Loads terminal capabilities for a terminal type.
tgetflag        // Gets a boolean capability (yes/no).
tgetnum         // Gets a numeric capability (e.g., max colors).
tgetstr         // Gets a control string capability (escape sequences).
tgoto           // Builds a cursor-movement string with coords.
tputs           // Outputs a capability string to the terminal safely.

*/
