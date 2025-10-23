/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaeklee <jaeklee@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 12:30:52 by jaeklee           #+#    #+#             */
/*   Updated: 2025/10/23 12:24:05 by jaeklee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	count_word(t_vec *tokens, size_t start)
{
	size_t	i;
	size_t	av;
	t_token	*token;

	i = start;
	av = 0;
	while (i < tokens->len)
	{
		token = (t_token *)ft_vec_get(tokens, i);
		if (token->type == PIPE)
			break ;
		if (token->type == WORD)
			av++;
		if (token->type == S_LT || token->type == S_GT || token->type == D_LT
			|| token->type == D_GT)
			i += 2;
		else
			i++;
	}
	return (av);
}

static int	handle_redirection(t_cmd *cmd, t_token *tok, t_token *next)
{
	if (!next || next->type != WORD)
	{
		perror("invalid token after redirection");
		return (0);
	}
	if (tok->type == S_LT)
	{
		if (cmd->input_fd)
			close(cmd->input_fd);
		cmd->input_fd = open(next->data, O_RDONLY);
	}
	else if (tok->type == S_GT)
		cmd->output_fd = open(next->data, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (tok->type == D_GT)
		cmd->output_fd = open(next->data, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else if (tok->type == D_LT)
	{
		if (!handle_heredoc(cmd, next->data))
			return (0);
		cmd->input_fd = open(cmd->heredoc_path, O_RDONLY);
	}
	if (cmd->input_fd == -1 || cmd->output_fd == -1)
	{
		perror("fd error");
		return (0);
	}
	return (1);
}

static char	**build_args(t_arena *arena, t_vec *tokens, size_t *i, t_cmd *cmd)
{
	int		num_of_av;
	char	**args;
	size_t	args_i;
	t_token	*tok;
	t_token	*next;

	args_i = 0;
	num_of_av = count_word(tokens, *i);
	args = arena_alloc(arena, sizeof(char *) * (num_of_av + 1));
	if (!args)
		return (NULL);
	while (*i < tokens->len)
	{
		tok = (t_token *)ft_vec_get(tokens, *i);
		if (tok->type == PIPE)
		{
			(*i)++;
			break ;
		}
		if (tok->type == S_LT || tok->type == S_GT || tok->type == D_LT
			|| tok->type == D_GT)
		{
			next = (t_token *)ft_vec_get(tokens, *i + 1);
			if (!handle_redirection(cmd, tok, next))
			{
				// free stuff ?? I dont't know yet
				return (NULL);
			}
			*i += 2;
		}
		else if (tok->type == WORD)
		{
			args[args_i] = arena_strdup(arena, tok->data, ft_strlen(tok->data));
			if (!args[args_i])
			{
				// free
				return (NULL);
			}
			args_i++;
			(*i)++;
		}
	}
	args[args_i] = NULL;
	return (args);
}

int	parse_tokens(t_arena *arena, t_vec *tokens, t_vec *cmds)
{
	size_t	i;
	t_cmd	cmd;
	char	**args;

	i = 0;
	ft_vec_new(cmds, 0, sizeof(t_cmd));
	while (i < tokens->len)
	{
		cmd.input_fd = 0;
		cmd.output_fd = 1;
		cmd.heredoc_path = NULL;
		args = build_args(arena, tokens, &i, &cmd);
		if (!args)
			perror("get args are failed");
		cmd.argv = args;
		if (ft_vec_push(cmds, &cmd) < 0)
			return (0);
	}
	return (1);
}
