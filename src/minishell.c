/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaeklee <jaeklee@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 10:23:07 by timurray          #+#    #+#             */
/*   Updated: 2025/12/02 15:19:04 by jaeklee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_signal;

static void	shell_loop(int interactive, t_info *info)
{
	char	*line;
	t_vec	tokens;
	t_vec	cmds;

	while (1)
	{
		line = read_line(interactive);
		if (!line)
			break ;
		if (g_signal == SIGINT)
			info->exit_code = 130;
		if (tokenizing(info, line, &tokens))
		{
			g_signal = 0;
			free(line);
			free_str_vec(&tokens);
			continue ;
		}
		if (parse_tokens(info, &tokens, &cmds))
		{
			g_signal = 0;
			free(line);
			ft_vec_free(&tokens);
			free_cmd_vec(&cmds);
			continue ;
		}
		free(line);
		ft_vec_free(&tokens);
		free_str_vec(&tokens);
		if (cmds.len != 0)
			info->exit_code = execute(&cmds, info);
		free_cmd_vec(&cmds);
		arena_free(info->arena);
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
	info.exit_code = 0;
	init_signals();
	shell_loop(isatty(STDIN_FILENO), &info);
	free_str_vec(&env);
	arena_free(&arena);
	return (EXIT_SUCCESS);
}

/*

"."/'m'"ini"s'h''e'"ll"
//solved

echo hello > a > b > c


Test
Applies to both of us:
Probably some exit code improvements
Mock evals with notes about issues.
Test with empty env before minishell starts.
*/
