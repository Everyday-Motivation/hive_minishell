/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizing_helper.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaeklee <jaeklee@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 13:06:08 by jaeklee           #+#    #+#             */
/*   Updated: 2025/12/03 18:55:55 by jaeklee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	handle_single_quote(char *input, size_t *i, char *buf)
{
	char	quote;
	size_t	buf_i;

	quote = input[(*i)++];
	buf_i = 0;
	while (input[*i] && input[*i] != quote)
		buf[buf_i++] = input[(*i)++];
	if (input[*i] == quote)
		(*i)++;
	buf[buf_i] = '\0';
	return (buf_i);
}

static size_t	collect_word(t_info *info, char *input, size_t *i, char **buf)
{
	size_t	buf_i;

	buf_i = 0;
	while (input[*i] && !ft_isspace(input[*i]) && !deli_check(input[*i]))
	{
		if (input[*i] == '\'')
			buf_i += handle_single_quote(input, i, &(*buf)[buf_i]);
		else if (input[*i] == '"')
		{
			buf_i = handle_double_quote(info, input, i, buf);
			if (buf_i == 0 && *buf[0] != '\0')
				buf_i = ft_strlen(*buf);
			else if (*buf[0] == '\0')
				(*buf)[buf_i++] = '\0';
		}
		else if (input[*i] == '$' && (*i == 0 || input[*i - 1] != '\\'))
			buf_i += handle_env_variable(info, input, i, buf);
		else
		{
			buf_i = ft_strlen(*buf);
			(*buf)[buf_i++] = input[(*i)++];
		}
	}
	return (buf_i);
}

void	init_word_token(size_t *buf_i, size_t *start, size_t i)
{
	*buf_i = 0;
	*start = i;
}

int	process_word(t_info *info, char *input, size_t *i, t_vec *tokens)
{
	char	*buf;
	size_t	buf_i;
	t_token	token;
	size_t	start;

	init_word_token(&buf_i, &start, *i);
	buf = arena_alloc(info->arena, ft_strlen(input) * 4);
	if (!buf)
		return (EXIT_FAILURE);
	buf_i = collect_word(info, input, i, &buf);
	if (buf_i == 0)
		return (EXIT_SUCCESS);
	token.type = WORD;
	token.data = arena_strdup(info->arena, buf, buf_i);
	token.raw_data = arena_strdup(info->arena, &input[start], *i - start);
	ft_vec_push(tokens, &token);
	return (EXIT_SUCCESS);
}
