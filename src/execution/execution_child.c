/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_child.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 14:24:26 by timurray          #+#    #+#             */
/*   Updated: 2025/11/28 14:25:07 by timurray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int reap_zombies(pid_t last_pid, size_t count_children)
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
		if(waited_pid == -1)
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
