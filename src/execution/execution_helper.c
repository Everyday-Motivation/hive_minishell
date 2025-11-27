/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_helper.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 12:58:29 by timurray          #+#    #+#             */
/*   Updated: 2025/11/27 15:50:30 by timurray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_pipes(int pipefd[2])
{
	close(pipefd[READ_END]);
	close(pipefd[WRITE_END]);
}

void	close_used_pipes(int pipefd[3])
{
	if (pipefd[PREV_READ] != -1)
		close(pipefd[PREV_READ]);
	if (pipefd[READ_END] != -1)
		close(pipefd[READ_END]);
	if (pipefd[WRITE_END] != -1)
		close(pipefd[WRITE_END]);
}

int	parent_builtin(t_vec *cmds, t_info *info, int pipefd[3])
{
	t_cmd	*cmd;
	int		status;

	cmd = (t_cmd *)ft_vec_get(cmds, 0);
	if (cmds->len == 1 && cmd->argv && cmd->argv[0] && is_bi(cmd->argv[0]) == 1)
	{
		save_std_fds(pipefd);
		if (handle_builtin_redirections(cmd) == EXIT_FAILURE)
		{
			reset_std_fds(pipefd);
			return (EXIT_FAILURE);
		}
		status = run_bi(cmd->argv, info, cmds);
		reset_std_fds(pipefd);
		return (status);
	}
	return (-1);
}
