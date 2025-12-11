/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 19:00:03 by timurray          #+#    #+#             */
/*   Updated: 2025/12/11 17:51:35 by timurray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exit_clear_rl_history(void)
{
	ft_putendl_fd("exit", 1);
	rl_clear_history();
}

int	return_error(int e)
{
	if (e == NO_BINARY)
		ft_putendl_fd("minishell cannot execute binaries.", 2);
	if (e == ENV_FAIL)
		ft_putendl_fd("Environment init failed", 2);
	if (e == ARENA_FAIL)
		ft_putendl_fd("arena init failed", 2);
	if (e == CD_ARGS_FAIL)
		ft_putendl_fd("minishell: cd: too many arguments", 2);
	return (EXIT_FAILURE);
}
