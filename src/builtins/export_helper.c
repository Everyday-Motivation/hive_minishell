/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_helper.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 17:47:02 by timurray          #+#    #+#             */
/*   Updated: 2025/11/02 15:23:15 by timurray         ###   ########.fr       */
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

static char	*get_valid_export_entry(char *line)
{
	size_t	i;
	int		has_equals;

	i = 0;
	has_equals = 0;
	while (line[i])
	{
		if (line[i] == '=')
			has_equals = 1;
		if (has_equals && line[i] == ' ')
			break ;
		i++;
	}
	return (ft_substr(line, 0u, i));
}

static char	*get_key_val(char *line)
{
	size_t	i;

	i = 0;
	while (!(line[i] == '='))
		i++;
	return (ft_substr(line, 0u, i + 1));
}

int	env_add_update_line(t_vec *env, char *val)
{
	char	*key;
	char	**line;
	char	*new_line;
	size_t	index;

	key = get_key_val(val);
	if (!key)
		return (0);
	new_line = get_valid_export_entry(val);
	if (!new_line)
		return (0);
	if (!str_in_str_vec(env, key))
	{
		if (!(ft_vec_push(env, &new_line) > 0))
			return (0);
	}
	else
	{
		index = get_str_index(env, key);
		line = (char **)ft_vec_get(env, index);
		free(*line);
		*line = new_line;
	}
	free(key);
	return (1);
}
