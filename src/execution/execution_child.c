/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_child.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 14:24:26 by timurray          #+#    #+#             */
/*   Updated: 2025/11/28 17:54:36 by timurray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	child_redirections(t_cmd *cmd, int pipefd[3])
{
	if (cmd->input_file)
	{
		pipefd[READ_END] = open(cmd->input_file, O_RDONLY);
		if (pipefd[READ_END] == -1)
		{
			perror("Filename");
			exit(1);
		}
		if (dup2(pipefd[READ_END], STDIN_FILENO) == -1)
		{
			perror("dup2 in file issue");
			close(pipefd[READ_END]);
			exit(1);
		}
		close(pipefd[READ_END]);
	}
	if (cmd->output_file)
	{
		if (cmd->append)
			pipefd[WRITE_END] = open(cmd->output_file,
					O_WRONLY | O_CREAT | O_APPEND, 0644);
		else
			pipefd[WRITE_END] = open(cmd->output_file,
					O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (pipefd[WRITE_END] == -1)
		{
			ft_putstr_fd("minishell: ", 2);
			perror(cmd->output_file);
			exit(1);
		}
		if (dup2(pipefd[WRITE_END], STDOUT_FILENO) == -1)
		{
			perror("dup2 out");
			close(pipefd[WRITE_END]);
			exit(1);
		}
		close(pipefd[WRITE_END]);
	}
}

void	child_pipes(t_cmd *cmd, int pipefd[3], size_t i, t_vec *cmds)
{
	if (cmd->input_file == NULL && cmd->heredoc_str == NULL && i > 0)
	{
		if (dup2(pipefd[PREV_READ], STDIN_FILENO) == -1)
		{
			perror("dup2 STDIN no in no here");
			exit(1);
		}
	}
	if (cmd->output_file == NULL && (i + 1 < cmds->len))
	{
		if (dup2(pipefd[WRITE_END], STDOUT_FILENO) == -1)
		{
			perror("dup2 STDOUT no out");
			exit(1);
		}
	}
	close_used_pipes(pipefd);
}

int	reap_zombies(pid_t last_pid, size_t count_children)
{
	size_t	reaped;
	pid_t	waited_pid;
	int		status;
	int		last_status;

	reaped = 0;
	last_status = 0;
	while (reaped < count_children)
	{
		waited_pid = wait(&status);
		if (waited_pid == -1)
		{
			perror("waited pid issue");
			return (EXIT_FAILURE);
		}
		if (waited_pid == last_pid)
			last_status = status;
		reaped++;
	}
	if (WIFEXITED(last_status))
		return (WEXITSTATUS(last_status));
	return (EXIT_FAILURE);
}
