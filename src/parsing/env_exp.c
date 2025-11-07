/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_exp.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaeklee <jaeklee@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 12:43:41 by jaeklee           #+#    #+#             */
/*   Updated: 2025/11/07 11:56:33 by jaeklee          ###   ########.fr       */
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
