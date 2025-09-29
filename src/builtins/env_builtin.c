/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 11:28:16 by timurray          #+#    #+#             */
/*   Updated: 2025/09/29 11:19:49 by timurray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	bi_env(char **av, t_vec *env)
{
	(void)av;
	if (!env)
		return (0);
	print_str_vec(env,"");
	return (1);
}

/* 
TODO: Arguments passed are added to print out but not added to env. Same names get overwritten.

 */