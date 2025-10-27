/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 10:38:09 by timurray          #+#    #+#             */
/*   Updated: 2025/10/27 11:10:30 by timurray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int exec(t_cmd *cmd)
{
	pid_t pid;
	int status;

	int in_fd;
	int out_fd;

	if (!cmd)
		return (EXIT_FAILURE);
	
	pid = fork();
	if (pid == -1)
	{
		perror("fork issue");
		return (EXIT_FAILURE);
	}
	
	if (pid == 0)
	{
		if (cmd->input_file)
		{
			in_fd = open(cmd->input_file, O_RDONLY);
			if (in_fd == -1)
			{
				perror("open in file issue");
				exit(1);
			}
			if (dup2(in_fd, STDIN_FILENO) == -1)
			{
				perror("dup2 in");
				close(in_fd);
				exit(1);
			}
			close(in_fd);
		}

		if (cmd->output_file)
		{
			if (cmd->append)
				out_fd = open(cmd->output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
			else
				out_fd = open(cmd->output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (out_fd == -1)
			{
				perror("open outfile issue");
				exit(1);
			}
			if(dup2(out_fd, STDOUT_FILENO) == -1)
			{
				perror("dup2 out");
				close(out_fd);
				exit(1);
			}
			close(out_fd);
		}

		//get path
		//execve 
		execvp(cmd->argv[0], cmd->argv);
		perror("execution error");
		exit(1);
	}
	
	
	if (waitpid(pid, &status, 0) == -1)
	{
		perror("wait pid child error in reaping");
		return (EXIT_FAILURE);
	}

	return (EXIT_SUCCESS);
}

// TODO: change to vector
// Env variables