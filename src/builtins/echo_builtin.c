/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 11:25:58 by timurray          #+#    #+#             */
/*   Updated: 2025/11/07 12:05:18 by timurray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	valid_n_flag(char *arg)
{
	size_t	i;

	if (!arg || arg[0] != '-')
		return (0);
	i = 1;
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

int	bi_echo(char **av, t_vec *env)
{
	size_t	i;
	int		newline;

	(void)env;
	i = 0;
	if (valid_n_flag(av[i]))
	{
		newline = false;
		i++;
	}
	else
		newline = true;
	while (av[i])
	{
		printf("%s", av[i]);
		i++;
	}
	if (newline)
		printf("\n");
	return (0);
}
