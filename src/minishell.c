/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaeklee <jaeklee@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 10:23:07 by timurray          #+#    #+#             */
/*   Updated: 2025/09/22 16:35:05 by jaeklee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// int	main_2(int ac, char **av, char **envp)
// {
// 	// t_vec	env;
// 	// char	*input;
// 	// t_arena	arena;

// 	// (void)ac;
// 	// (void)av;
// 	// if (!arena_init(&arena))
// 	// {
// 	// 	perror("arena init failed");
// 	// 	return (EXIT_FAILURE);
// 	// }
// 	// if (!init_env(&env, envp))
// 	// {
// 	// 	perror("Environment init failed");
// 	// 	return (EXIT_FAILURE);
// 	// }
// 	// while (1)
// 	// {
// 		if (!check_input(&arena, &input))
// 	// 		break ;
// 	// 	if (strcmp(input, "exit") == 0)
// 	// 	{
// 	// 		break ;
// 	// 	}
// 	// 	tokenizing(input);
// 		// printf("%s\n", input);
// 		// TODO: 여기서 parsing, tokenizing 등 처리
// 		arena.size = 0;
// 	// }
// 	// arena_free(&arena);
// 	// 환경 변수 해제
// 	// TODO: free_env(env);
// 	/// signal init
// 	// get input -> check if it is valid input
// 	// -> if yes ->parsing  ->>> first tokenizing
// 	return (EXIT_SUCCESS);
// }

static void	shell_loop(int interactive, t_arena *arena)
{
	char	*line;

	while (1)
	{
		line = read_line(interactive);
		if (!line)
			break ;
		printf("read line: %s\n", line);
		tokenizing(arena, line);
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
	shell_loop(isatty(STDIN_FILENO), &arena);
	arena_free(&arena);
	return (EXIT_SUCCESS);
}
