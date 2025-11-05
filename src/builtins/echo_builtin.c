/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 11:25:58 by timurray          #+#    #+#             */
/*   Updated: 2025/11/05 16:41:14 by timurray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int valid_n_flag(char *arg)
{
	size_t i;

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

int bi_echo(char **av, t_vec *env)
{
	size_t i;

	i = 0;
	(void)env;
	while(av[i])
	{
		if (valid_n_flag(av[i]))
		{
				
		}
		//TODO: check for -n flag
		//TODO: print the rest
		i++;
	}
	return (0);
}
/* 
TODO: if valid n flag, skip it?
TODO: write content
TODO: write content if "-n" is detected
 */