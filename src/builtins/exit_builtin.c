/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 11:28:30 by timurray          #+#    #+#             */
/*   Updated: 2025/11/13 11:52:33 by timurray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	bi_exit(char **av, t_vec *env)
{
	(void)av;
	(void)env;
	
	exit(1);
	return (1);
}

/*
TODO: Should clean up things.
TODO: Handle arguments (numeric, non-numeric, too many)
*/
