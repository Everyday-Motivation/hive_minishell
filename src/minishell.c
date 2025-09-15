/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 10:23:07 by timurray          #+#    #+#             */
/*   Updated: 2025/09/15 18:27:14 by timurray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	shell_loop(void)
{
	char	*line;
	char	*prompt;
	char	*cwd;

	while (1)
	{
		cwd = getcwd(NULL, 0);
		prompt = ft_strjoin(cwd, "$ ");
		free(cwd);
		line = readline(prompt);
		free(prompt);
		if (!line)
		{
			ft_putendl_fd("exit", 2);
			exit(0);
		}
		if (*line)
			add_history(line);
		free(line);
	}
}

void	sigint_handler(int signal)
{
	if (signal == SIGINT)
	{
		write(STDOUT_FILENO, "^C", 2);
		write(STDOUT_FILENO, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}

void	set_sig_action(void)
{
	struct sigaction	sa;

	ft_bzero(&sa, sizeof(sa));
	sa.sa_handler = sigint_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}

int	main(void)
{
	rl_catch_signals = 0;
	set_sig_action();
	shell_loop();
	return (EXIT_SUCCESS);
}
