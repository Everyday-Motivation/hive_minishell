/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_exp.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaeklee <jaeklee@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 12:43:41 by jaeklee           #+#    #+#             */
/*   Updated: 2025/11/24 16:04:18 by jaeklee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_env_value(t_vec *env, const char *key)
{
	size_t	i;
	size_t	name_len;
	char	*entry;

	i = 0;
	if (!key || key[0] == '\0')
		return (NULL);
	name_len = ft_strlen(key);
	while (i < env->len)
	{
		entry = *(char **)ft_vec_get(env, i);
		if (entry && strncmp(entry, key, name_len) == 0
			&& entry[name_len] == '=')
			return (entry + name_len + 1);
		i++;
	}
	return (NULL);
}

void	init_word_token(size_t *buf_i, size_t *start, size_t i)
{
	*buf_i = 0;
	*start = i;
}

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

size_t	handle_env_variable(t_info *info, char *input, size_t *i, char **buf)
{
	char	*key;
	char	*val;
	size_t	key_len;

	if (!input[*i + 1] || (!ft_isalnum(input[*i + 1]) && input[*i + 1] != '_'
			&& input[*i + 1] != '?'))
	{
		append_env_value(info, buf, "$");
		(*i)++;
		return (1);
	}
	if (input[*i + 1] == '?')
	{
		(*i)++;
		return (handle_exit_status_variable(info, i, buf));
	}
	key = extract_env_key(input, i);
	if (!key)
		return (0);
	key_len = ft_strlen(key);
	val = get_env_value(info->env, key);
	free(key);
	if (val)
		return (append_env_value(info, buf, val));
	else
		return (key_len);
}
