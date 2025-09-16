/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 10:23:07 by timurray          #+#    #+#             */
/*   Updated: 2025/09/16 19:55:18 by timurray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"


int	main(int ac, char **av, char **envp)
{
	t_vec	env;
	char	*input;
	t_arena	arena;

	(void)ac;
	(void)av;
	if (!arena_init(&arena))
	{
		perror("arena init failed");
		return (EXIT_FAILURE);
	}
	if (!init_env(&env, envp))
	{
		perror("Environment init failed");
		return (EXIT_FAILURE);
	}
	while (1)
	{
		if (!check_input(&arena, &input))
			break ;
		if (strcmp(input, "exit") == 0)
		{
			break ;
		}
		tokenizing(input);
		// printf("%s\n", input);
		// TODO: 여기서 parsing, tokenizing 등 처리
		arena.size = 0;
	}
	arena_free(&arena);
	// 환경 변수 해제
	// TODO: free_env(env);
	/// signal init
	// get input -> check if it is valid input
	// -> if yes ->parsing  ->>> first tokenizing
	return (EXIT_SUCCESS);
}

// static void	shell_loop(int interactive)
// {
// 	char	*line;

// 	while (1)
// 	{
// 		line = read_line(interactive);
// 		if (!line)
// 			break ;
// 		printf("read line: %s\n", line);
// 		// TODO: execute/tokenize/etc(line);
// 		free(line);
// 	}
// }

// int	main(int ac, char **av, char **envp)
// {
// 	(void)av;
// 	(void)envp;
// 	if (ac != 1)
// 	{
// 		ft_putendl_fd("minishell cannot execute binaries.", 2);
// 		return (1);
// 	}
// 	init_signals();
// 	shell_loop(isatty(STDIN_FILENO));
// 	return (0);
// }
