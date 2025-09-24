/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 10:23:07 by timurray          #+#    #+#             */
/*   Updated: 2025/09/24 12:52:55 by timurray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	shell_loop(int interactive, char **envp)
{
	char	*line;
	t_vec	lex_line;
	t_vec	parse;

	while (1)
	{
		line = read_line(interactive);
		if (!line)
			break ;
		// printf("read line: %s\n", line);
		ft_vec_new(&lex_line, 0, sizeof(t_lex));
		lexer(line, &lex_line);
		lex_to_parse(&lex_line, &parse);
		free(line);
	}
}

int	main(int ac, char **av, char **envp)
{
	t_vec	env;
	t_arena	arena;

	(void)av;
	(void)envp;
	if (ac != 1)
		return (return_error(NO_BINARY));
	if (!init_env(&env, envp))
		return (return_error(ENV_FAIL));
	if (!arena_init(&arena))
		return (return_error(ARENA_FAIL));
	init_signals();
	shell_loop(isatty(STDIN_FILENO), envp);
	arena_free(&arena);
	return (EXIT_SUCCESS);
}
