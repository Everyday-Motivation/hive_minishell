/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_pipes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 15:49:35 by timurray          #+#    #+#             */
/*   Updated: 2025/11/27 15:50:34 by timurray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_pipes(int pipefd[3])
{
	pipefd[READ_END] = -1;
	pipefd[WRITE_END] = -1;
	pipefd[PREV_READ] = -1;
}

void	save_std_fds(int pipefd[2])
{
	pipefd[READ_END] = dup(STDIN_FILENO);
	pipefd[WRITE_END] = dup(STDOUT_FILENO);
}

void	reset_std_fds(int pipefd[2])
{
	dup2(pipefd[READ_END], STDIN_FILENO);
	dup2(pipefd[WRITE_END], STDOUT_FILENO);
	close(pipefd[READ_END]);
	close(pipefd[WRITE_END]);
}

int	signal_pipe(int pipefd[3], size_t i, t_vec *cmds)
{
	parent_sig();
	if (i + 1 < cmds->len)
	{
		if (pipe(pipefd) == -1)
		{
			perror("pipe issue");
			if (pipefd[PREV_READ] != -1)
				close(pipefd[PREV_READ]);
			return (EXIT_FAILURE);
		}
	}
	else
	{
		pipefd[READ_END] = -1;
		pipefd[WRITE_END] = -1;
	}
	return (EXIT_SUCCESS);
}