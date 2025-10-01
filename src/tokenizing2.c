/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizing2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaeklee <jaeklee@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 13:06:08 by jaeklee           #+#    #+#             */
/*   Updated: 2025/10/01 13:08:26 by jaeklee          ###   ########.fr       */
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

size_t	handle_double_quote(t_info *info, char *input, size_t *i, char *buf)
{
	char	quote;
	size_t	start;
	size_t	buf_i;
	char	*temp;

	quote = input[(*i)++];
	start = *i;
	buf_i = 0;
	while (input[*i] && input[*i] != quote)
		(*i)++;
	temp = arena_strdup(info->arena, &input[start], *i - start);
	if (input[*i] == quote)
		(*i)++;
	temp = expand_env(info->arena, temp, info->env);
	while (temp[buf_i])
	{
		buf[buf_i] = temp[buf_i];
		buf_i++;
	}
	buf[buf_i] = '\0';
	return (buf_i);
}

size_t	handle_env_variable(t_info *info, char *input, size_t *i, char *buf)
{
	size_t	var_start;
	size_t	buf_i;
	char	*key;
	char	*val;

	var_start = ++(*i);
	buf_i = 0;
	while (input[*i] && (ft_isalnum(input[*i]) || input[*i] == '_'))
		(*i)++;
	key = arena_strdup(info->arena, &input[var_start], *i - var_start);
	val = get_env_value(info->env, key);
	if (val)
	{
		while (val[buf_i])
		{
			buf[buf_i] = val[buf_i];
			buf_i++;
		}
	}
	buf[buf_i] = '\0';
	return (buf_i);
}

void	process_word(t_info *info, char *input, size_t *i, t_vec *tokens)
{
	char	buf[1024];
	size_t	buf_i;
	t_token	token;

	buf_i = 0;
	while (input[*i] && !ft_isspace(input[*i]) && !deli_check(input[*i]))
	{
		if (input[*i] == '\'')
			buf_i += handle_single_quote(input, i, &buf[buf_i]);
		else if (input[*i] == '"')
			buf_i += handle_double_quote(info, input, i, &buf[buf_i]);
		else if (input[*i] == '$' && (*i == 0 || input[*i - 1] != '\\'))
			buf_i += handle_env_variable(info, input, i, &buf[buf_i]);
		else
		{
			buf[buf_i++] = input[(*i)++];
			continue ;
		}
	}
	buf[buf_i] = '\0';
	token.type = WORD;
	token.data = arena_strdup(info->arena, buf, buf_i);
	ft_vec_push(tokens, &token);
}
