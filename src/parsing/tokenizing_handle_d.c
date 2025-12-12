/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizing_handle_d.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaeklee <jaeklee@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 15:18:16 by jaeklee           #+#    #+#             */
/*   Updated: 2025/12/12 13:38:55 by jaeklee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	handle_double_quote_content(t_info *info, char *input, size_t *i,
		char **buf)
{
	size_t	buf_i;
	char	next;

	buf_i = ft_strlen(*buf);
	while (input[*i] && input[*i] != '"')
	{
		if (input[*i] == '$')
		{
			next = input[*i + 1];
			if (!(ft_isalnum(next) || next == '_' || next == '?'))
				(*buf)[buf_i++] = input[(*i)++];
			else
			{
				buf_i += handle_env_variable(info, input, i, buf);
			}
		}
		else
			(*buf)[buf_i++] = input[(*i)++];
	}
	if (input[*i] == '"')
		(*i)++;
	return (buf_i);
}

size_t	handle_double_quote(t_info *info, char *input, size_t *i, char **buf)
{
	char	quote;
	size_t	buf_i;

	quote = input[(*i)++];
	buf_i = ft_strlen(*buf);
	if (input[*i] == quote)
	{
		(*i)++;
		if (*buf == NULL || buf_i == 0)
		{
			(*buf)[buf_i] = '\0';
			return (1);
		}
		return (0);
	}
	return (handle_double_quote_content(info, input, i, buf));
}

void	free_split(char **split)
{
	int	i;

	i = 0;
	if (!split)
		return ;
	while (split[i])
		free(split[i++]);
	free(split);
}

void	split_word_tokens(t_info *info, t_vec *tokens, t_token *token)
{
	char	**parts;
	int		i;
	t_token	new_tok;

	if (!ft_strchr(token->data, ' '))
	{
		ft_vec_push(tokens, token);
		return ;
	}
	parts = ft_split(token->data, ' ');
	if (!parts)
		return ;
	i = 0;
	while (parts[i])
	{
		new_tok.type = WORD;
		new_tok.data = arena_strdup(info->arena, parts[i], ft_strlen(parts[i]));
		new_tok.raw_data = token->raw_data;
		ft_vec_push(tokens, &new_tok);
		i++;
	}
	free_split(parts);
}
