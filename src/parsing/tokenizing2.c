/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizing2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaeklee <jaeklee@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 13:06:08 by jaeklee           #+#    #+#             */
/*   Updated: 2025/11/05 16:07:44 by jaeklee          ###   ########.fr       */
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

size_t	handle_double_quote(t_info *info, char *input, size_t *i, char **buf)
{
	char	quote;
	size_t	buf_i;

	quote = input[(*i)++];
	buf_i = 0;
	while (input[*i] && input[*i] != quote)
	{
		if (input[*i] == '$' && input[*i + 1])
		{
			buf_i += handle_env_variable(info, input, i, buf);
		}
		else
		{
			(*buf)[buf_i++] = input[(*i)++];
		}
	}
	if (input[*i] == quote)
		(*i)++;
	return (buf_i);
}

size_t	handle_exit_status_variable(t_info *info, size_t *i, char **buf)
{
	char	*val;
	int		val_len;
	char	*temp;
	size_t	buf_len;

	(*i)++;
	val = ft_itoa(info->exit_code);
	val_len = ft_strlen(val);
	buf_len = ft_strlen(*buf);
	temp = arena_alloc(info->arena, buf_len + val_len + 1);
	ft_memcpy(temp, *buf, buf_len);
	ft_memcpy(temp + buf_len, val, val_len);
	temp[buf_len + val_len] = '\0';
	*buf = temp;
	return (val_len);
}

size_t	handle_env_variable(t_info *info, char *input, size_t *i, char **buf)
{
	size_t	start;
	char	*key;
	char	*val;
	int		val_len;
	char	*temp;

	start = ++(*i);
	if (input[*i] == '?')
		return (handle_exit_status_variable(info, i, buf));
	while (input[*i] && (ft_isalnum(input[*i]) || input[*i] == '_'))
		(*i)++;
	key = arena_strdup(info->arena, &input[start], *i - start);
	val = get_env_value(info->env, key);
	if (val)
	{
		val_len = ft_strlen(val);
		temp = arena_alloc(info->arena, val_len + ft_strlen(*buf) + 1);
		ft_memcpy(temp, *buf, ft_strlen(*buf));
		ft_memcpy(temp + ft_strlen(*buf), val, val_len);
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
	size_t	start;

	buf_i = 0;
	start = *i;
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
			buf[buf_i++] = input[(*i)++];
	}
	buf[buf_i] = '\0';
	token.type = WORD;
	token.data = arena_strdup(info->arena, buf, buf_i);
	token.raw_data = arena_strdup(info->arena, &input[start], *i - start);
	ft_vec_push(tokens, &token);
}
