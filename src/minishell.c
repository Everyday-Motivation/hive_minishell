/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 10:23:07 by timurray          #+#    #+#             */
/*   Updated: 2025/09/16 19:47:55 by timurray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	shell_loop(int interactive)
{
	char	*line;

	while (1)
	{
		line = read_line(interactive);
		if (!line)
			break ;
		printf("read line: %s\n", line);
		// TODO: execute/tokenize/etc(line);
		free(line);
	}
}

int	main(int ac, char **av, char **envp)
{
	(void)av;
	(void)envp;
	if (ac != 1)
	{
		ft_putendl_fd("minishell cannot execute binaries.", 2);
		return (1);
	}
	init_signals();
	shell_loop(isatty(STDIN_FILENO));
	return (0);
}
