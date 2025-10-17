/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 16:40:59 by timurray          #+#    #+#             */
/*   Updated: 2025/10/17 11:29:23 by timurray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/* 
typedef struct s_cmd
{
	char			**argv; //cmds to process
	int				input_fd;
	int				output_fd;
	int				heredoc;
}					t_cmd;

typedef struct s_info {
	t_arena *arena;
	t_vec   *env;
} t_info;
 */

int executor(void) //add commands and env for path
{
	int i;
	pid_t pid;
	int pipefd[2];
	int tmpfd;
	
	// ls | grep txt | wc -l
	char *cmd[][4] = {
		{"ls", NULL}, 
		{"grep", "txt", NULL},
		{"wc","-l", NULL}
	};
	int n;
	n = 3;

	pipefd[0] = -1;
	pipefd[1] = -1;
	tmpfd = -1;

	i = 0;
	while(i < n)
	{
		if (i < n -1)
		{
			if(pipe(pipefd) == -1)
			{
				perror("pipe issue");
				exit(1);
			}
		}

		pid = fork();
		if(pid < 0)
		{
			perror("fork issue");
			exit(1);
		}

		if(pid == 0)
		{
			if (tmpfd != -1)
			{
				if (dup2(tmpfd, STDIN_FILENO) == -1)
				{
					perror("dup2 error on in");
					exit(1);
				}
			}

			if (i < n -1)
			{
				if(dup2(pipefd[1], STDOUT_FILENO) == -1)
				{
					perror("dup2 error on out");
					exit(1);
				}
			}

			if(tmpfd != -1)
				close(tmpfd);
			if(pipefd[0] != -1)
				close(pipefd[0]);
			if(pipefd[1] != -1)
				close(pipefd[1]);
			
			execvp(cmd[i][0], cmd[i]); //change to execve
			exit(127);
		}


		if(tmpfd != -1)
		{
			close(tmpfd);
			tmpfd = -1;
		}
		if(i < n - 1)
		{
			close(pipefd[1]);
			tmpfd = pipefd[0];
		}


		i++;
	}


	if (tmpfd != -1)
		close(tmpfd);
	

	i = 0;
	while (i < n)
	{
		wait(NULL);
		// printf("n=%i\n",i);
		i++;
	}

	return (1);
}
