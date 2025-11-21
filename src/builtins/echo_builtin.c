/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 11:25:58 by timurray          #+#    #+#             */
/*   Updated: 2025/11/21 19:10:33 by timurray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	valid_n_flag(char *arg, bool *newline)
{
	size_t	i;

	if (!arg || arg[0] != '-')
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

	(void)info->env;
	i = 0;
	newline = true;
	if (!av || !av[0])
	{
		printf("\n");
		return (0);
	}
	if (valid_n_flag(av[i], &newline))
		i++;
	while (av[i])
	{
		printf("%s", av[i]);
		if (av[i + 1] != NULL)
			printf(" ");
		i++;
	}
	if (newline)
		printf("\n");
	return (0);
}
