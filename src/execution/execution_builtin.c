/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_builtin.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 14:49:28 by timurray          #+#    #+#             */
/*   Updated: 2025/11/21 19:13:35 by timurray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_bi(char *cmd)
{
	if (ft_strcmp(cmd, "cd") == 0)
		return (1);
	if (ft_strcmp(cmd, "echo") == 0)
		return (1);
	if (ft_strcmp(cmd, "env") == 0)
		return (1);
	if (ft_strcmp(cmd, "exit") == 0)
		return (1);
	if (ft_strcmp(cmd, "export") == 0)
		return (1);
	if (ft_strcmp(cmd, "pwd") == 0)
		return (1);
	if (ft_strcmp(cmd, "unset") == 0)
		return (1);
	return (0);
}

// int	run_bi(char **argv, t_vec *env)
int	run_bi(char **argv, t_info *info)
{
	if (ft_strcmp(argv[0], "cd") == 0)
		return (bi_cd(argv + 1, info));
	if (ft_strcmp(argv[0], "echo") == 0)
		return (bi_echo(argv + 1, info));
	if (ft_strcmp(argv[0], "env") == 0)
		return (bi_env(argv + 1, info));
	if (ft_strcmp(argv[0], "exit") == 0)
		return (bi_exit(argv + 1, info));
	if (ft_strcmp(argv[0], "export") == 0)
		return (bi_export(argv + 1, info));
	if (ft_strcmp(argv[0], "pwd") == 0)
		return (bi_pwd(argv + 1, info));
	if (ft_strcmp(argv[0], "unset") == 0)
		return (bi_unset(argv + 1, info));
	return (EXIT_FAILURE);
}
