/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaeklee <jaeklee@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 10:23:07 by timurray          #+#    #+#             */
/*   Updated: 2025/10/23 12:26:46 by jaeklee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void shell_loop(int interactive, t_info *info)
{
	char *line;
	t_vec tokens;
	t_vec cmds;

	while (1)
	{
		line = read_line(interactive);
		if (!line)
			break;

		// printf("read line: %s\n", line);

		if (!tokenizing(info, line, &tokens))
		{
			printf("Tokenizing failed\n");
			free(line);
			continue;
		}

		//: tokens → cmds
		if (!parse_tokens(info->arena, &tokens, &cmds))
		{
			printf("Parsing failed\n");
			free(line);
			ft_vec_free(&tokens);
			continue;
		}
		
		execute_cmds(&cmds, info->env);

		free(line);
		ft_vec_free(&tokens);
		ft_vec_free(&cmds);
	}
}


int	main(int ac, char **av, char **envp)
{
	t_info	info;
	t_vec	env;
	t_arena	arena;

	(void)av;
	if (ac != 1)
		return (return_error(NO_BINARY));

	if (!init_env(&env, envp))
		return (return_error(ENV_FAIL));

	if (!arena_init(&arena))
		return (return_error(ARENA_FAIL));

	info.arena = &arena;
	info.env = &env;

	init_signals();
	shell_loop(isatty(STDIN_FILENO), &info);

	arena_free(&arena);
	return (EXIT_SUCCESS);
}


// int	main(int ac, char **av, char **envp)
// {
// 	t_vec	env;
// 	t_arena	arena;

// 	(void)av;
// 	(void)envp;
// 	if (ac != 1)
// 		return (return_error(NO_BINARY));
// 	if (!init_env(&env, envp))
// 		return (return_error(ENV_FAIL));
// 	if (!arena_init(&arena))
// 		return (return_error(ARENA_FAIL));
// 	init_signals();
// 	shell_loop(isatty(STDIN_FILENO), &arena, &env);
// 	arena_free(&arena);
// 	return (EXIT_SUCCESS);
// }


