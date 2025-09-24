/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaeklee <jaeklee@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 10:23:07 by timurray          #+#    #+#             */
/*   Updated: 2025/09/24 17:48:53 by jaeklee          ###   ########.fr       */
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
    t_vec cmds;

    while (1)
    {
        line = read_line(interactive);
        if (!line)
            break;

        printf("read line: %s\n", line);

        // 1. 토큰화
        if (!tokenizing(arena, line, &tokens, env))
        {
            printf("Tokenizing failed\n");
            free(line);
            continue;
        }

        // 2. 파싱: tokens → cmds
        if (!parse_tokens(arena, &tokens, &cmds))
        {
            printf("Parsing failed\n");
            free(line);
            ft_vec_free(&tokens); // 토큰 정리
            continue;
        }

        // 3. 디버그: 파싱된 명령어 출력
        for (size_t i = 0; i < cmds.len; i++)
        {
            t_cmd *cmd = ft_vec_get(&cmds, i);
            printf("cmd[%zu]:\n", i);
            for (size_t j = 0; cmd->argv[j]; j++)
                printf("  argv[%zu]: %s\n", j, cmd->argv[j]);
            printf("  input_fd: %d, output_fd: %d, heredoc: %d\n", cmd->input_fd, cmd->output_fd, cmd->heredoc);
        }

        // 4. 나중에 execute_pipeline(&cmds, env); 호출 예정
		execute_cmds(&cmds, env);

        free(line);
        ft_vec_free(&tokens);
        ft_vec_free(&cmds);
        arena_free(arena); 
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


