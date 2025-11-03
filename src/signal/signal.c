/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 10:52:10 by timurray          #+#    #+#             */
/*   Updated: 2025/11/03 17:04:56 by timurray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sigint_handler(int signal)
{
	if (signal == SIGINT)
	{
		write(STDOUT_FILENO, "^C", 2);
		write(STDOUT_FILENO, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
		g_signal = 1;
	}
}

void	init_signals(void)
{
	struct sigaction	sa;

	rl_catch_signals = 0;
	ft_bzero(&sa, sizeof(sa));
	sa.sa_handler = sigint_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}
//TODO: May be necessary to make g_signal = SIGINT and other signals