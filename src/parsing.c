/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaeklee <jaeklee@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 12:30:52 by jaeklee           #+#    #+#             */
/*   Updated: 2025/09/29 12:31:23 by jaeklee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int handle_redirection(t_arena *arena, t_token *tok, t_token *next, t_cmd *cmd)
{
	(void)arena;
	
	if (!next || next->type != WORD)
		return 0;

	if (tok->type == S_LT)
		cmd->input_fd = open(next->data, O_RDONLY);
	else if (tok->type == S_GT)
		cmd->output_fd = open(next->data, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (tok->type == D_GT)
		cmd->output_fd = open(next->data, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else if (tok->type == D_LT)
	{
		cmd->heredoc = 1;
		//  heredoc 처리
	}
	if ((tok->type == S_LT || tok->type == S_GT || tok->type == D_GT) && (cmd->input_fd == -1 || cmd->output_fd == -1))
	{
		perror("open");
		return 0;
	}
	return 1;
}

static char **vec_to_argv(t_arena *arena, t_vec *argv)
{
    char **args;
    size_t j;
	
	args = arena_alloc(arena, sizeof(char *) * (argv->len + 1));
	j= 0;
    while (j < argv->len)
    {
        args[j] = *(char **)ft_vec_get(argv, j);
        j++;
    }
    args[argv->len] = NULL;
    return args;
}

static int parse_single_command(t_arena *arena, t_vec *tokens, size_t *i, t_cmd *cmd)
{
	t_token *tok;
	t_token *next;
	t_vec argv;
	char *tmp;

	cmd->input_fd = 0;
	cmd->output_fd = 1;
	cmd->heredoc = 0;

	if (ft_vec_new(&argv, 0, sizeof(char *)) < 0)
		return 0;
	while (*i < tokens->len)
	{
		tok = (t_token *)ft_vec_get(tokens, *i);

		if (tok->type == PIPE)
		{
			(*i)++;
			break;
		}
		else if (tok->type == S_LT || tok->type == S_GT || tok->type == D_LT || tok->type == D_GT)
		{
			next = ft_vec_get(tokens, *i + 1);
			if (!handle_redirection(arena, tok, next, cmd))
				return 0;
			(*i) += 2;
		}
		else if (tok->type == WORD)
		{
			tmp = arena_strdup(arena, tok->data, strlen(tok->data));
			ft_vec_push(&argv, &tmp);
			(*i)++;
		}
		else
			(*i)++;
	}
	cmd->argv = vec_to_argv(arena, &argv);
	return 1;
}

int parse_tokens(t_arena *arena, t_vec *tokens, t_vec *cmds)
{
	t_cmd cmd;
	size_t i = 0;

	if (ft_vec_new(cmds, 0, sizeof(t_cmd)) < 0)
		return 0;

	while (i < tokens->len)
	{
		if (!parse_single_command(arena, tokens, &i, &cmd))
			return 0;
		ft_vec_push(cmds, &cmd);
	}

	return 1;
}
