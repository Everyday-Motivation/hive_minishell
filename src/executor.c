/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 12:30:46 by timurray          #+#    #+#             */
/*   Updated: 2025/10/26 14:52:45 by timurray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"


// parent process
// If no outfile, dup2 fd_out, STDOUT
// If no infile, dup2 fd_in, STDIN
// If

int lsh_launch(char **args)
{
	pid_t pid;
	pid_t wpid;
	int status;

	pid = fork();
	if (pid == 0)
	{ //CHILD
		if(execvp(args[0], args) == -1)
		{
			perror("shell launch");
		}
		exit(EXIT_FAILURE);
	}
	else if (pid < 0)
	{
		perror("fork issue");
	}
	else
	{ //PARENT
		do
		{
			wpid = waitpid(pid, &status, WUNTRACED);
		} while (!WIFEXITED(status) && !WIFSIGNALED(status));
	}
	return (1);
}

char *builtin_str[] = {
	"env",
	"unset",
	"export"
};

int (*builtin_func[]) (char **) = {
	&bi_env,
	&bi_export,
	&bi_unset
};

int lsh_num_builtins()
{
	return sizeof(builtin_str) / sizeof(char *);
}

int lsh_execute(char **args)
{
	int i;

	if(args[0] == NULL)
	{
		return (1);
	}

	i = 0;
	while (i < lsh_num_builtins())
	{
		if(ft_strncmp(args[0], builtin_str[i], ft_strlen(args[0])+1)==0)
		{
			return (*builtin_func[i])(args);
		}
		i++;
	}
	return lsh_launch(args);
}
















/* 
TODO: Get Path
TODO: Get Env char **
TODO: Built in or external
TODO: Waitpid, Reap child process
TODO: Redirections
TODO: Which struct
TODO:
 */