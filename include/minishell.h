/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaeklee <jaeklee@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 12:44:00 by timurray          #+#    #+#             */
/*   Updated: 2025/11/03 11:30:51 by jaeklee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define ARENA_INIT_SIZE 16386

# include "../libft/libft.h"
# include <dirent.h>
# include <fcntl.h>
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

extern volatile sig_atomic_t	g_signal;

enum							e_pipe_end
{
	READ_END = 0,
	WRITE_END = 1,
	PREV_READ = 2
};

enum							e_child
{
	CHILD = 0
};

enum							e_error_code
{
	NO_BINARY = 0,
	ENV_FAIL = 1,
	ARENA_FAIL = 2,
};

typedef struct s_cmd
{
	char						**argv;
	char						*input_file;
	char						*output_file;
	char						*heredoc_str;
	bool						append;
	int							heredoc_counter;
}								t_cmd;

typedef enum e_token_type
{
	WORD,
	S_LT,
	D_LT,
	S_GT,
	D_GT,
	PIPE
}								t_token_type;
typedef struct s_arena
{
	struct s_arena				*next;
	char						*block;
	size_t						size;
	size_t						capacity;
}								t_arena;

typedef struct s_info
{
	t_arena						*arena;
	t_vec						*env;
}								t_info;

typedef struct s_token
{
	t_token_type				type;
	char						*data;
}								t_token;

// Builtins
int								bi_unset(char **av, t_vec *env);
int								bi_export(char **av, t_vec *env);
int								bi_env(char **av, t_vec *env);

// Utils
int								sort_vec_str_ptr(t_vec *v);
int								copy_vec_str_ptr(t_vec *env_cpy, t_vec *env);
int								sort_vec_str_ptr(t_vec *v);
int								env_add_update_line(t_vec *env, char *val);

// Signal
void							sigint_handler(int signal);
void							init_signals(void);
void							init_hd_signals(void);
void							heredoc_sigint_handler(int signal);

// Arena
int								arena_init(t_arena *arena);
void							arena_free(t_arena *arena);
void							*arena_alloc(t_arena *arena, size_t n);
char							*arena_strdup(t_arena *arena, const char *s,
									size_t n);

// Input
int								get_input(t_arena *arena, char **input);
int								check_input(t_arena *arena, char **input);

// Env
int								init_env(t_vec *env, char **envp);
int								copy_env(t_vec *env, char **envp);
int								add_pwd(t_vec *env);
int								increment_shlvl(t_vec *env);

// env_expanding
char							*get_env_value(t_vec *env,
									const char *var_name);

// Tokenizing
int								tokenizing(t_info *info, char *input,
									t_vec *tokens);
int								deli_check(char c);
int								quote_check(char *input, size_t *i);

void							process_word(t_info *info, char *input,
									size_t *i, t_vec *tokens);
size_t							handle_env_variable(t_info *info, char *input,
									size_t *i, char **buf);
size_t							handle_double_quote(t_info *info, char *input,
									size_t *i, char **buf);
size_t							handle_single_quote(char *input, size_t *i,
									char *buf);

// parsing
int								parse_tokens(t_arena *arena, t_vec *tokens,
									t_vec *cmds);
char							**build_args(t_arena *arena, t_vec *tokens,
									size_t *i, t_cmd *cmd);
int								handle_pipe(t_token *tok, size_t *i);
int								handle_ridir(t_vec *tokens, t_token *tok,
									size_t *i, t_cmd *cmd);
int								handle_redirection(t_cmd *cmd, t_token *tok, t_token *next);
// heredoc
int								handle_heredoc(t_cmd *cmd, const char *limiter);
void							count_heredoc(t_arena *arena, t_vec *tokens,
									t_vec *cmds);
// Prompt
char							*read_line(int interactive);

// execute
int								execute(t_vec *cmds, t_vec *env);
void							process_heredoc_str(t_cmd *cmd);
void							close_pipes(int pipefd[2]);

// find_path
char							*ft_strjoin_3(const char *s1, const char *s2,
									const char *s3);
void							ft_free_split(char **arr);
char							*get_path_env(t_vec *env);
char							*find_executable_in_paths(char *path_env,
									char *cmd);
char							*search_path(char *cmd, t_vec *env);

// Error
void							exit_clear_rl_history(void);
int								return_error(int e);

// Vec helpers
void							free_str_vec(t_vec *str_vec);
int								str_in_str_vec(t_vec *str_vec, char *str);
void							print_str_vec(t_vec *str_vec, char *prefix);
int								vec_remove_str(t_vec *src, size_t index);
size_t							get_str_index(t_vec *src, char *s);
char							**vec_to_arr(t_vec *v);

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
