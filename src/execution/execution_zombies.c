/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_zombies.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 11:24:55 by timurray          #+#    #+#             */
/*   Updated: 2025/12/12 11:29:04 by timurray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	status_code(int status)
{
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGQUIT)
			ft_putstr_fd("Quit (core dumped)\n", 2);
		if (WTERMSIG(status) == SIGINT)
			ft_putstr_fd("\n", 2);
		return (128 + WTERMSIG(status));
	}
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (EXIT_FAILURE);
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
			if (errno == ECHILD && reaped == count_children)
				break ;
			perror("waited pid issue");
			return (EXIT_FAILURE);
		}
		if (waited_pid == last_pid)
			last_status = status;
		reaped++;
	}
	return (status_code(last_status));
}
