/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 11:25:58 by timurray          #+#    #+#             */
/*   Updated: 2025/11/02 15:22:59 by timurray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int builtin_echo(char **av, t_vec *env)
{
	size_t i;

	i = 0;
	(void)env;
	while(av[i])
	{
		
		i++;
	}
	return (0);
}

