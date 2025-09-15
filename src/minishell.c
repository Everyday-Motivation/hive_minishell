/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaeklee <jaeklee@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 10:23:07 by timurray          #+#    #+#             */
/*   Updated: 2025/09/14 16:39:58 by jaeklee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	main(int ac, char **av, char **envp)
{
	(void)ac;
	(void)av;
	char **env;
	char *input;
	t_arena arena;

	if (!init_env(&env, envp))
	{
		perror("Environment init failed");
		return (EXIT_FAILURE);
	}
	
	 while (1)
    {
        if (!check_input(&arena, &input))
            break;
        if (strcmp(input, "exit") == 0)
        {
            break;
        }

        // TODO: 여기서 parsing, tokenizing 등 처리
        printf("입력받음: %s\n", input);

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
