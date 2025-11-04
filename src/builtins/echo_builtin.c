/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 11:25:58 by timurray          #+#    #+#             */
/*   Updated: 2025/11/04 15:13:52 by timurray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int bi_echo(char **av, t_vec *env)
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
/* 
TODO: write content
TODO: write content if "-n" is detecgted
 */