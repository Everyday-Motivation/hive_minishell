/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 11:28:16 by timurray          #+#    #+#             */
/*   Updated: 2025/09/28 17:32:02 by timurray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	bi_env(char **av, t_vec *env)
{
	(void)av;
	if (!env)
		return (0);
	print_str_vec(env);
	return (1);
}
