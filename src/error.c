
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 19:00:03 by timurray          #+#    #+#             */
/*   Updated: 2025/09/17 10:44:28 by timurray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	exit_clear_rl_history(void)
{
	ft_putendl_fd("exit", 2);
	rl_clear_history();
	exit(EXIT_SUCCESS);
}

int	return_error(int e)
{
	if (e == NO_BINARY)
		ft_putendl_fd("minishell cannot execute binaries.", 2);
	if (e == ENV_FAIL)
		perror("Environment init failed");
	if (e == ARENA_FAIL)
		perror("arena init failed");
	return (EXIT_FAILURE);
}
