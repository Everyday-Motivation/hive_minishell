/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_helper.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 17:47:02 by timurray          #+#    #+#             */
/*   Updated: 2025/11/19 11:50:56 by timurray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	copy_vec_str_ptr(t_vec *env_cpy, t_vec *env)
{
	size_t	i;
	char	**line;

	i = 0;
	while (i < env->len)
	{
		line = (char **)ft_vec_get(env, i);
		if (!(ft_vec_push(env_cpy, line) > 0))
			return (0);
		i++;
	}
	return (1);
}

static char	*get_key_val(char *line)
{
	size_t	i;

	i = 0;
	while (line[i] && line[i] != '=')
		i++;
	return (ft_substr(line, 0u, i));
}

int	add_env(t_vec *env, char *key, char *av)
{
	char	*dup;

	dup = ft_strdup(av);
	if (!dup)
	{
		free(key);
		return (0);
	}
	if (!(ft_vec_push(env, &dup) > 0))
	{
		free(dup);
		free(key);
		return (0);
	}
	return (1);
}

int	replace_env(t_vec *env, char *key, char *av)
{
	size_t	index;
	char	**line;

	index = get_str_index(env, key);
	line = (char **)ft_vec_get(env, index);
	if (!line)
	{
		free(key);
		return (0);
	}
	free(*line);
	*line = ft_strdup(av);
	if (!*line)
	{
		free(key);
		return (0);
	}
	return (1);
}

int	env_add_update_line(t_vec *env, char *av)
{
	char	*key;

	key = get_key_val(av);
	if (!key)
		return (0);
	if (!str_in_str_vec(env, key))
	{
		if (add_env(env, key, av) == 0)
			return (0);
	}
	else
	{
		if (replace_env(env, key, av) == 0)
			return (0);
	}
	free(key);
	return (1);
}
