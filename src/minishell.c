/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaeklee <jaeklee@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 10:23:07 by timurray          #+#    #+#             */
/*   Updated: 2025/12/02 17:41:24 by jaeklee          ###   ########.fr       */
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
	init_env(&env, envp);
	// {
	// 	return (return_error(ENV_FAIL));
	// }
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
echo hello""world // solved
echo hello > a > b > c


Test
Applies to both of us:
Probably some exit code improvements
Mock evals with notes about issues.as
Test with empty env before minishell starts.
*/


//  % env -i ./minishell 
// /home/jaeklee/projects/Rank3/minishell$ asd
// =================================================================
// ==920505==ERROR: AddressSanitizer: requested allocation size 0x79415e1363845d08 (0x79415e1363846d08 after adjustments for alignment, red zones etc.) exceeds maximum supported size of 0x10000000000 (thread T0)
//     #0 0x49a28d in malloc (/home/jaeklee/projects/Rank3/minishell/minishell+0x49a28d)
//     #1 0x4e8642 in vec_to_arr /home/jaeklee/projects/Rank3/minishell/src/utils/vec_to_arr.c:25:8
//     #2 0x4d8d12 in child_run /home/jaeklee/projects/Rank3/minishell/src/execution/execution.c:47:8
//     #3 0x4d9d31 in child_process /home/jaeklee/projects/Rank3/minishell/src/execution/execution.c:89:2
//     #4 0x4d9fe5 in execute /home/jaeklee/projects/Rank3/minishell/src/execution/execution.c:112:4
//     #5 0x4cb970 in shell_loop /home/jaeklee/projects/Rank3/minishell/src/minishell.c:49:22
//     #6 0x4cb585 in main /home/jaeklee/projects/Rank3/minishell/src/minishell.c:74:2
//     #7 0x72825ec29d8f in __libc_start_call_main csu/../sysdeps/nptl/libc_start_call_main.h:58:16

// ==920505==HINT: if you don't care about these errors you may set allocator_may_return_null=1
// SUMMARY: AddressSanitizer: allocation-size-too-big (/home/jaeklee/projects/Rank3/minishell/minishell+0x49a28d) in malloc

//cat << eof"" / fixed
