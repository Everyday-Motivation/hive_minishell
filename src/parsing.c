/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaeklee <jaeklee@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 12:30:52 by jaeklee           #+#    #+#             */
/*   Updated: 2025/10/30 16:51:45 by jaeklee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	count_word(t_vec *tokens, size_t start)
{
	size_t	i;
	size_t	words;
	t_token	*token;

	i = start;
	words = 0;
	while (i < tokens->len)
	{
		token = (t_token *)ft_vec_get(tokens, i);
		if (token->type == PIPE)
			break ;
		if (token->type == WORD)
			words++;
		if (token->type == S_LT || token->type == S_GT || token->type == D_LT
			|| token->type == D_GT)
			i += 2;
		else
			i++;
	}
	return (words);
}

int	handle_redirection(t_cmd *cmd, t_token *tok, t_token *next)
{
	if (!next || next->type != WORD)
		return (EXIT_FAILURE);
	if (tok->type == S_LT)
		cmd->input_file = next->data;
	else if (tok->type == S_GT)
	{
		cmd->output_file = next->data;
		cmd->append = false;
	}
	else if (tok->type == D_GT)
	{
		cmd->output_file = next->data;
		cmd->append = true;
	}
	else if (tok->type == D_LT)
	{
		init_hd_signals();
		if (!handle_heredoc(cmd, next->data))
		{
			init_signals();
			return (EXIT_FAILURE);
		}
		init_signals();
	}
	return (EXIT_SUCCESS);
}

char	**build_args(t_arena *arena, t_vec *tokens, size_t *i, t_cmd *cmd)
{
	int		num_of_av;
	char	**args;
	size_t	args_i;
	t_token	*tok;

	args_i = 0;
	num_of_av = count_word(tokens, *i);
	args = arena_alloc(arena, sizeof(char *) * (num_of_av + 1));
	if (!args)
		return (NULL);
	while (*i < tokens->len)
	{
		tok = ft_vec_get(tokens, *i);
		if (handle_pipe(tok, i) == EXIT_SUCCESS)
			break ;
		if (handle_ridir(tokens, tok, i, cmd) == -1)
			return (NULL);
		if (tok->type == WORD)
		{
			args[args_i++] = tok->data;
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
	count_heredoc(arena, tokens, cmds);
	while (i < tokens->len)
	{
		ft_memset(&cmd, 0, sizeof(t_cmd));
		args = build_args(arena, tokens, &i, &cmd);
		if (!args)
		{
			ft_putendl_fd("syntax error near unexpected token `newline'", 2);
			return (EXIT_FAILURE);
		}
		cmd.argv = args;
		if (ft_vec_push(cmds, &cmd) < 0)
			return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

void	count_heredoc(t_arena *arena, t_vec *tokens, t_vec *cmds)
{
	t_token	*tok;
	size_t	i;
	size_t	heredoc_counter;

	heredoc_counter = 0;
	i = 0;
	while (i < tokens->len)
	{
		tok = (t_token *)ft_vec_get(tokens, i);
		if (tok->type == D_LT)
			heredoc_counter++;
		if (heredoc_counter > 16)
		{
			ft_putendl_fd("maximum here-document count exceeded", 2);
			ft_vec_free(tokens);
			ft_vec_free(cmds);
			arena_free(arena);
			exit(2);
		}
		i++;
	}
}
