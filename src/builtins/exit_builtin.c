/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 11:28:30 by timurray          #+#    #+#             */
/*   Updated: 2025/11/24 15:27:22 by timurray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_exit(t_info *info, t_vec *cmds, int code)
{
	size_t	i;
	t_cmd	*cmd;

	free_str_vec(info->env);
	if (cmds)
	{
		i = 0;
		while (i < cmds->len)
		{
			cmd = (t_cmd *)ft_vec_get(cmds, i);
			if (cmd->heredoc_str)
				free(cmd->heredoc_str);
			i++;
		}
		ft_vec_free(cmds);
	}
	arena_free(info->arena);
	exit(code);
}

static int	is_all_num(char *av)
{
	size_t	i;

	i = 0;
	if (av[i] == '-' || av[i] == '+')
		i++;
	if (av[i] == '\0')
		return (0);
	while (av[i])
	{
		if (!ft_isdigit(av[i]))
			return (0);
		i++;
	}
	return (1);
}

int	bi_exit(char **av, t_info *info, t_vec *cmds)
{
	int	exit_code;

	ft_putendl_fd("exit", 1);
	if (!av || !av[0])
		free_exit(info, cmds, info->exit_code);
	if (av[1])
	{
		ft_putendl_fd("minishell: exit: too many arguments", 2);
		return (EXIT_FAILURE);
	}
	if (av[0])
	{
		if (!is_all_num(av[0]) || ft_atoi_check(av[0], &exit_code) == 0)
		{
			ft_putstr_fd("minishell: exit: ", 2);
			ft_putstr_fd(av[0], 2);
			ft_putendl_fd(": numeric argument required", 2);
			free_exit(info, cmds, 2);
		}
		else
			free_exit(info, cmds, exit_code % 256);
	}
	free_exit(info, cmds, 0);
	return (EXIT_SUCCESS);
}
