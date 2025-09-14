/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 10:23:07 by timurray          #+#    #+#             */
/*   Updated: 2025/09/14 13:41:57 by timurray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void shell_loop(void)
{
	char *line;
	int loop_status;

	loop_status = 1;
	while(loop_status)
	{
		line = readline("$ ");
		if (!line)
		{
			ft_putendl_fd("exit.", 2);
			exit(0);
		}
		if(*line)
			add_history(line);
		free(line);
	}
}

void sigint_handler(int signal)
{
	if (signal == SIGINT)
	{
		rl_replace_line("^C", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}

void set_sig_action(void)
{
	struct sigaction sa;

	ft_bzero(&sa, sizeof(sa));
	sa.sa_handler = sigint_handler;
	sigaction(SIGINT, &sa, NULL);
}

int	main(void)
{
	set_sig_action();
	shell_loop();
	return (EXIT_SUCCESS);
}
