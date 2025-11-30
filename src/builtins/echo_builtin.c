/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 11:25:58 by timurray          #+#    #+#             */
/*   Updated: 2025/11/30 19:01:09 by timurray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	valid_n_flag(char *arg, bool *newline)
{
	size_t	i;

	if (!arg || arg[0] != '-' || arg[1] != 'n')
		return (0);
	i = 1;
	while (arg[i])
	{
		if (arg[i] != 'n')
		{
			*newline = true;
			return (0);
		}
		i++;
	}
	*newline = false;
	return (1);
}

int	bi_echo(char **av, t_info *info)
{
	size_t	i;
	bool	newline;

	(void)info;
	i = 0;
	newline = true;
	if (!av || !av[0])
	{
		ft_putstr_fd("\n", 1);
		return (0);
	}
	while (valid_n_flag(av[i], &newline))
		i++;
	while (av[i])
	{
		ft_putstr_fd(av[i], 1);
		if (av[i + 1] != NULL)
			ft_putstr_fd(" ", 1);
		i++;
	}
	if (newline)
		ft_putendl_fd("", 1);
	return (0);
}
