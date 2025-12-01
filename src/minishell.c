/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 10:23:07 by timurray          #+#    #+#             */
/*   Updated: 2025/12/01 08:48:42 by timurray         ###   ########.fr       */
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

I just need some help understand what is being sent here.

#1
---
$EMPTY

is it just argv[0][0] == '\0'?
or is there something else?

I've put in the code you suggested but it isn't full working:
execution.c line 80:	if (cmd->argv == NULL || cmd->argv[0] == NULL || cmd->argv[0][0] == '\0')

Here's the challenge:
If I run:

$doesnotexist
mini:
bash:
✅



$doesnotexist echo hi
mini:
bash: hi
❌



"$doesnotexist" echo hi
mini:
bash: command not found
❌



'$doesnotexist' echo hi
mnini: command not found
bash: command not found
✅



$sdfsd | $dsfsd | echo hi
mini: hi
bash: hi
✅



$doesnotexist $HOME
mini:
bash: /home/timurray: is a directory
❌



echo $doesnotexist $HOME
mini:  /home/timurray
bash: /home/timurray
❌ NOTE: here our minishell has an extra space before /home/timurray



echo $NOTHING hello $NOTHING2 world!
mini:  hello  world!
bash: hello world!
❌




Could you explain from parsing what the difference between these cases so I can handle them properly?

---



#2
---
data_arena.c has norm issues. Not the 25 line limit kind.



#3
---
Complex error. There is an issue I'm dealing with in execution with command not found leaking. So you'll encounter that, don't worry.
The issue to highlight here is the following.

If I run:
cat | cat | cat | cat |

I will get:
syntax error near unexpected token

Not if it I exit with "exit".
There is memory that is definitely lost.

I assume with a syntax it doesn't go to execution? It may mean something needs to be freed in parsing?

*/

