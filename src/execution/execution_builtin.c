/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_builtin.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 14:49:28 by timurray          #+#    #+#             */
/*   Updated: 2025/11/28 14:14:45 by timurray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	parent_builtin(t_vec *cmds, t_info *info)
{
	t_cmd *cmd;
	int status;
	int saved_fds[2];
	int is_exit;

	cmd = (t_cmd *)ft_vec_get(cmds, 0);
	if (cmds->len == 1 && cmd->argv && cmd->argv[0] && is_bi(cmd->argv[0]) == 1)
	{
		is_exit = (ft_strcmp(cmd->argv[0], "exit") == 0);
		if (!is_exit)
			save_std_fds(saved_fds);
		if (handle_builtin_redirections(cmd) == EXIT_FAILURE)
		{
			reset_std_fds(saved_fds);
			return (EXIT_FAILURE);
		}
		status = run_bi(cmd->argv, info, cmds);
		if (!is_exit)
			reset_std_fds(saved_fds);
		return (status);
	}
	return (-1);
}