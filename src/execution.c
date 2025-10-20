/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 16:40:59 by timurray          #+#    #+#             */
/*   Updated: 2025/10/20 12:43:21 by timurray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/*
typedef struct s_cmd
{
	char			**argv; //cmds to process
	int	input_fd;
	int	output_fd;
	int	heredoc;
}		t_cmd;

typedef struct s_info {
	t_arena *arena;
	t_vec   *env;
}		t_info;
 */

// TODO: pass env
// TODO: execute in parent cd, exit, export, unset, pwd, echo


int	executor(t_vec *cmd) 
{
	size_t i;
	pid_t pid;

	int tmpfd;
	int pipefd[2];
	
	t_cmd *command;
	
	int need_pipe;
	int in_fd;
	int out_fd;

	pipefd[0] = -1;
	pipefd[1] = -1;
	tmpfd = -1;

	i = 0;
	while (i < cmd->len)
	{
		command = (t_cmd *)ft_vec_get(cmd, i);
		need_pipe = 0;
		if (i < cmd->len - 1)
		{
			need_pipe = 1;
			if (pipe(pipefd) == -1)
			{
				perror("pipe issue");
				if(tmpfd != -1)
				{
					close(tmpfd);
					tmpfd = -1;
				}
				return (0);
			}
		}


		pid = fork();
		if (pid < 0)
		{
			perror("fork issue, pid less than 0");
			if(tmpfd != -1)
			{
				close(tmpfd);
				tmpfd = -1;
			}
			if (need_pipe)
			{
				close(pipefd[0]);
				close(pipefd[1]);
			}
			return (0);
		}



		if (pid == 0) //child process
		{
			
			in_fd = command->input_fd;
			if (in_fd == -1)
				in_fd = tmpfd;
			out_fd = command->output_fd;
			if(out_fd == -1 && need_pipe)
				out_fd = pipefd[1];

			
			if (in_fd != -1 && in_fd != STDIN_FILENO)
			{
				if (dup2(in_fd, STDIN_FILENO) == -1)
				{
					perror("dup2 infd to STDIN");
					exit(1);
				}
			}


			if(out_fd != -1 && out_fd != STDOUT_FILENO)
			{
				if (dup2(out_fd, STDOUT_FILENO) == -1)
				{
					perror("dup2 outfd to STDOUT");
					exit(1);
				}
			}

			if(tmpfd != -1)
			{
				close(tmpfd);
				tmpfd = -1;
			}


			if(need_pipe)
			{
				close(pipefd[0]);
				close(pipefd[1]);
			}

			if(command->input_fd != -1)
				close(command->input_fd);
			if(command->output_fd != -1)
				close(command->output_fd);

			execvp(command->argv[0], command->argv);
			perror(command->argv[0]);
			exit(127);
		}


		if(tmpfd != -1)
		{
			close(tmpfd);
			tmpfd = -1;
		}
		if(need_pipe)
		{
			close(pipefd[1]);
			tmpfd = pipefd[0];
		}
		if(command->input_fd != -1)
			close(command->input_fd);
		if(command->output_fd != -1)
			close(command->output_fd);
		
		i++;
	}

	if (tmpfd != -1)
	{
		close(tmpfd);
		tmpfd = -1;
	}



	i = 0;
	while (i < cmd->len)
	{
		wait(NULL);
		i++;
	}


	return (1);
}

/* 	// ls | grep txt | wc -l
	char *cmd[][4] = {
		{"ls", NULL},
		{"grep", "txt", NULL},
		{"wc","-l", NULL},
		{NULL}
	}; */