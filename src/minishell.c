/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 10:23:07 by timurray          #+#    #+#             */
/*   Updated: 2025/09/16 11:20:28 by timurray         ###   ########.fr       */
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
			rl_clear_history();
			exit(0);
		}
		if (*line)
			add_history(line);
		free(line);
	}
}

int	main(void)
{
	rl_catch_signals = 0;
	set_sig_action();
	shell_loop();
	return (EXIT_SUCCESS);
}
