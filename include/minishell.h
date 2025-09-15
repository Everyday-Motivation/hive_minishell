/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaeklee <jaeklee@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 10:23:55 by timurray          #+#    #+#             */
/*   Updated: 2025/09/15 15:42:44 by jaeklee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <dirent.h>
# include <termios.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <string.h>
# include <sys/ioctl.h>
# include <termcap.h>


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
readline
rl_clear_history
rl_on_new_line
rl_replace_line
rl_redisplay
add_history

<stdio.h>
printf
perror

<stdlib.h>
malloc
free
exit
getenv

<unistd.h>
write
access
open
read
close
	fork 
 	execve
	dup
dup2
	pipe
	isatty
ttyname
ttyslot
getcwd
chdir
unlink

<sys/wait.h>
	wait
	waitpid
wait3
wait4

<signal.h>
	signal
	sigaction
	sigemptyset
	sigaddset
	kill

<sys/stat.h>
stat
lstat
fstat

<dirent.h>
opendi
readdir
closedir

<string.h>
strerror

<sys/ioctl.h>
ioctl

<termios.h>
tcsetattr
tcgetattr

<termcap.h>
tgetent
tgetflag
tgetnum
tgetstr
tgoto
tputs
 */
