/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_exp.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaeklee <jaeklee@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 12:43:41 by jaeklee           #+#    #+#             */
/*   Updated: 2025/12/02 17:56:27 by jaeklee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*extract_env_key(char *input, size_t *i)
{
	size_t	start;
	size_t	key_len;
	char	*key;

	start = ++(*i);
	while (input[*i] && (ft_isalnum(input[*i]) || input[*i] == '_'))
		(*i)++;
	key_len = *i - start;
	key = malloc(key_len + 1);
	if (!key)
		return (NULL);
	ft_memcpy(key, &input[start], key_len);
	key[key_len] = '\0';
	return (key);
}

static size_t	append_env_value(t_info *info, char **buf, const char *val)
{
	size_t	buf_len;
	char	*temp;

	if (!val)
		return (0);
	buf_len = ft_strlen(*buf);
	temp = arena_alloc(info->arena, buf_len + ft_strlen(val) + 1);
	ft_memmove(temp, *buf, buf_len);
	ft_memmove(temp + buf_len, val, ft_strlen(val));
	temp[buf_len + ft_strlen(val)] = '\0';
	*buf = temp;
	return (ft_strlen(val));
}

static size_t	append_dollar(t_info *info, char **buf, size_t *i)
{
	append_env_value(info, buf, "$");
	(*i)++;
	return (1);
}

static size_t	append_dollar_with_literal(t_info *info, char **buf,
		char *input, size_t *i)
{
	size_t	start;
	size_t	len;
	size_t	ret;
	char	*tmp;

	start = *i;
	(*i)++;
	while (input[*i] && !ft_isspace(input[*i]) && !deli_check(input[*i]))
		(*i)++;
	len = *i - start;
	tmp = malloc(len + 1);
	if (!tmp)
		return (0);
	ft_memcpy(tmp, &input[start], len);
	tmp[len] = '\0';
	ret = append_env_value(info, buf, tmp);
	free(tmp);
	return (ret);
}

size_t	handle_env_variable(t_info *info, char *input, size_t *i, char **buf)
{
	char	*key;
	char	*val;

	if (input[*i] == '$' && (input[*i + 1] == '\0' || input[*i + 1] == '\''))
		return (append_dollar(info, buf, i));
	if (input[*i + 1] == '?')
	{
		(*i)++;
		return (handle_exit_status_variable(info, i, buf));
	}
	if (ft_isalnum(input[*i + 1]) || input[*i + 1] == '_')
	{
		key = extract_env_key(input, i);
		if (!key)
			return (0);
		val = get_env_value(info->env, key);
		free(key);
		if (val)
			return (append_env_value(info, buf, val));
		return (0);
	}
	return (append_dollar_with_literal(info, buf, input, i));
}
