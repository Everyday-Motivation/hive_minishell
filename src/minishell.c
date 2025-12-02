/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaeklee <jaeklee@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 10:23:07 by timurray          #+#    #+#             */
/*   Updated: 2025/12/02 11:08:09 by jaeklee          ###   ########.fr       */
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
			continue ;
		}
		if (parse_tokens(info, &tokens, &cmds))
		{
			g_signal = 0;
			free(line);
			ft_vec_free(&tokens);
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
TEST

#1
---
data_arena.c has norm issues. Not the 25 line limit kind.

#2
Memory leak on syntax error:
$> valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes --suppressions=readline.supp -s ./minishell
minishell$ cat | cat |
minishell$ exit


==980324== Memcheck, a memory error detector
==980324== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==980324== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
==980324== Command: ./minishell
==980324== 
/home/timurray/Documents/projects/dev/minishell$ cat | cat |
syntax error near unexpected token
/home/timurray/Documents/projects/dev/minishell$ exit
exit
==980324== 
==980324== FILE DESCRIPTORS: 3 open (3 std) at exit.
==980324== 
==980324== HEAP SUMMARY:
==980324==     in use at exit: 208,394 bytes in 229 blocks
==980324==   total heap usage: 526 allocs, 297 frees, 258,855 bytes allocated
==980324== 
==980324== 48 bytes in 1 blocks are definitely lost in loss record 17 of 67
==980324==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==980324==    by 0x408516: ft_vec_new (in /home/timurray/Documents/projects/dev/minishell/minishell)
==980324==    by 0x4087B5: ft_vec_push (in /home/timurray/Documents/projects/dev/minishell/minishell)
==980324==    by 0x40280F: parse_tokens (parsing.c:118)
==980324==    by 0x401479: shell_loop (minishell.c:36)
==980324==    by 0x4013CC: main (minishell.c:70)
==980324== 
==980324== LEAK SUMMARY:
==980324==    definitely lost: 48 bytes in 1 blocks
==980324==    indirectly lost: 0 bytes in 0 blocks
==980324==      possibly lost: 0 bytes in 0 blocks
==980324==    still reachable: 0 bytes in 0 blocks
==980324==         suppressed: 208,346 bytes in 228 blocks
==980324== 
==980324== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)
--980324-- 
--980324-- used_suppression:     62 leak readline readline.supp:2 suppressed: 204,241 bytes in 221 blocks
--980324-- used_suppression:      4 leak add_history readline.supp:8 suppressed: 4,105 bytes in 7 blocks
==980324== 
==980324== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)
*/

