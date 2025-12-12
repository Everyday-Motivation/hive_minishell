/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 10:38:09 by timurray          #+#    #+#             */
/*   Updated: 2025/12/12 12:11:13 by timurray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	close_prev_read(int pipefd[3])
{
	if (pipefd[PREV_READ] != -1)
		close(pipefd[PREV_READ]);
}

static int	run_pipeline(t_vec *cmds, t_info *info, int pipefd[3],
		pid_t *last_pid)
{
	size_t	i;
	pid_t	pid;

	i = 0;
	while (i < cmds->len)
	{
		if (signal_pipe(pipefd, i, cmds) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		pid = fork();
		if (pid == -1)
			return (fork_error(pipefd));
		if (pid == CHILD)
			child_process(cmds, info, pipefd, i);
		*last_pid = pid;
		recycle_pipes(pipefd, i++, cmds);
	}
	return (EXIT_SUCCESS);
}

int	execute(t_vec *cmds, t_info *info)
{
	int		pipefd[3];
	int		status;
	pid_t	last_pid;

	init_pipes(pipefd);
	status = parent_builtin(cmds, info);
	if (status != -1)
		return (status);
	wait_sig();
	if (cmds->len == 0)
		return (init_signals(), EXIT_SUCCESS);
	status = run_pipeline(cmds, info, pipefd, &last_pid);
	if (status != EXIT_SUCCESS)
	{
		close_prev_read(pipefd);
		init_signals();
		return (status);
	}
	close_prev_read(pipefd);
	status = reap_zombies(last_pid, cmds->len);
	init_signals();
	return (status);
}
