/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizing2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaeklee <jaeklee@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 13:06:08 by jaeklee           #+#    #+#             */
/*   Updated: 2025/10/23 12:22:53 by jaeklee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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

size_t	handle_double_quote(t_info *info, char *input, size_t *i, char **buf)
{
	char	quote;
	size_t	start;
	size_t	buf_i;
	char	*temp;
	size_t	temp_i;

	quote = input[(*i)++];
	start = *i;
	buf_i = 0;
	while (input[*i] && input[*i] != quote)
		(*i)++;
	temp = arena_strdup(info->arena, &input[start], *i - start);
	if (input[*i] == quote)
		(*i)++;
	if (temp[0] == '$')
	{
		temp_i = 0;
		return (handle_env_variable(info, temp, &temp_i, buf));
	}
	while (temp[buf_i] && temp[buf_i] != '$')
	{
		(*buf)[buf_i] = temp[buf_i];
		buf_i++;
	}
	return (buf_i);
}

size_t	handle_env_variable(t_info *info, char *input, size_t *i, char **buf)
{
	size_t	start;
	char	*key;
	char	*val;
	int		val_len;
	char	*temp;

	start = ++(*i);
	while (input[*i] && (ft_isalnum(input[*i]) || input[*i] == '_'))
		(*i)++;
	key = arena_strdup(info->arena, &input[start], *i - start);
	val = get_env_value(info->env, key);
	if (val)
	{
		val_len = ft_strlen(val);
		temp = arena_alloc(info->arena, val_len + 1);
		ft_memcpy(temp, val, val_len);
		*buf = temp;
		return (val_len);
	}
	return (ft_strlen(key));
}

void	process_word(t_info *info, char *input, size_t *i, t_vec *tokens)
{
	char	*buf;
	size_t	buf_i;
	t_token	token;

	buf_i = 0;
	buf = arena_alloc(info->arena, ft_strlen(input));
	while (input[*i] && !ft_isspace(input[*i]) && !deli_check(input[*i]))
	{
		if (input[*i] == '\'')
			buf_i += handle_single_quote(input, i, &buf[buf_i]);
		else if (input[*i] == '"')
			buf_i += handle_double_quote(info, input, i, &buf);
		else if (input[*i] == '$' && (*i == 0 || input[*i - 1] != '\\'))
			buf_i += handle_env_variable(info, input, i, &buf);
		else
		{
			buf[buf_i++] = input[(*i)++];
		}
	}
	buf[buf_i] = '\0';
	token.type = WORD;
	token.data = arena_strdup(info->arena, buf, buf_i);
	ft_vec_push(tokens, &token);
}
