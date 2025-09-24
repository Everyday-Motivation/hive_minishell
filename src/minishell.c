/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaeklee <jaeklee@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 10:23:07 by timurray          #+#    #+#             */
/*   Updated: 2025/09/24 13:08:55 by jaeklee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// static void	shell_loop(int interactive, t_arena *arena, t_vec *env)
// {
// 	char	*line;
	// t_vec	lex_line;
	// t_vec	parse;

// 	while (1)
// 	{
// 		line = read_line(interactive);
// 		if (!line)
// 			break ;
// 		printf("read line: %s\n", line);
// 		tokenizing(arena, line, env);
// 		free(line);
// 	}
// }

static void shell_loop(int interactive, t_arena *arena, t_vec *env)
{
    char *line;
    t_vec tokens;
	char	*line;


    while (1)
    {
        line = read_line(interactive);
        if (!line)
            break;

        printf("read line: %s\n", line);

        if (!tokenizing(arena, line, &tokens, env))
        {
            printf("Tokenizing failed\n");
            free(line);
            continue;
        }
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
	shell_loop(isatty(STDIN_FILENO), &arena, &env);
	arena_free(&arena);
	return (EXIT_SUCCESS);
}


