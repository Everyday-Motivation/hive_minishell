/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaeklee <jaeklee@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 10:52:10 by timurray          #+#    #+#             */
/*   Updated: 2025/11/03 11:12:32 by jaeklee          ###   ########.fr       */
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
		g_signal = SIGINT;
	}
}

void	init_signals(void)
{
	struct sigaction	sa;

	rl_done = false;
	rl_catch_signals = 0;
	ft_bzero(&sa, sizeof(sa));
	sa.sa_handler = sigint_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}

static int	heredoc_event_hook(void)
{
	if (g_signal == SIGINT)
	{
		rl_done = true;
		return (1);
	}
	return (0);
}

void	heredoc_sigint_handler(int signal)
{
	if (signal == SIGINT)
	{
		write(STDOUT_FILENO, "^C", 2);
		rl_replace_line("", 0);
		rl_on_new_line();
		g_signal = SIGINT;
	}
}

void	init_hd_signals(void)
{
	struct sigaction	sa;

	rl_event_hook = heredoc_event_hook;
	rl_catch_signals = 0;
	ft_bzero(&sa, sizeof(sa));
	sa.sa_handler = heredoc_sigint_handler;
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}
