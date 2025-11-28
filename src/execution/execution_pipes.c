/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_pipes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 15:49:35 by timurray          #+#    #+#             */
/*   Updated: 2025/11/28 14:18:30 by timurray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_pipes(int pipefd[3])
{
	pipefd[READ_END] = -1;
	pipefd[WRITE_END] = -1;
	pipefd[PREV_READ] = -1;
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

void recycle_pipes(int pipefd[3], size_t i, t_vec *cmds)
{
	if (pipefd[PREV_READ] != -1)
	{
		close(pipefd[PREV_READ]);
		pipefd[PREV_READ] = -1;
	}
	if(i + 1 < cmds->len)
	{
		if(pipefd[WRITE_END] != -1)
		{
			close(pipefd[WRITE_END]);
			pipefd[WRITE_END] = -1;
		}
		pipefd[PREV_READ] = pipefd[READ_END];
		pipefd[READ_END] = -1;
	}
	else
	{
		pipefd[READ_END] = -1;
		if(pipefd[WRITE_END] != -1)
		{
			close(pipefd[WRITE_END]);
			pipefd[WRITE_END] = -1;
		}
	}
}

void	close_pipes(int pipefd[2])
{
	close(pipefd[READ_END]);
	close(pipefd[WRITE_END]);
}

void	close_used_pipes(int pipefd[3])
{
	if (pipefd[READ_END] != -1)
	{
		close(pipefd[READ_END]);
		pipefd[READ_END] = -1;
	}
	if (pipefd[WRITE_END] != -1)
	{
		close(pipefd[WRITE_END]);
		pipefd[WRITE_END] = -1;
	}
	if (pipefd[PREV_READ] != -1)
	{
		close(pipefd[PREV_READ]);
		pipefd[PREV_READ] = -1;
	}
}