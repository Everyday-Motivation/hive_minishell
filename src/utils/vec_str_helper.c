/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_str_helper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 10:45:57 by timurray          #+#    #+#             */
/*   Updated: 2025/11/28 15:29:26 by timurray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_str_vec(t_vec *str_vec)
{
	size_t	i;
	char	*str;

	if (!str_vec || !str_vec->memory)
		return ;
	i = 0;
	while (i < str_vec->len)
	{
		str = *(char **)ft_vec_get(str_vec, i);
		free(str);
		i++;
	}
	ft_vec_free(str_vec);
}

int	str_in_str_vec(t_vec *str_vec, char *str)
{
	size_t	i;
	char	*line;
	size_t	len;

	if (!str_vec || !str)
		return (0);
	len = ft_strlen(str);
	i = 0;
	while (i < str_vec->len)
	{
		line = *(char **)ft_vec_get(str_vec, i);
		if (line && ft_strncmp(line, str, len) == 0 && (line[len] == '='
				|| line[len] == '\0'))
			return (1);
		i++;
	}
	return (0);
}

void	print_str_vec(t_vec *str_vec, char *prefix)
{
	size_t	i;
	char	*line;
	char	*prefix_line;

	i = 0;
	while (i < str_vec->len)
	{
		line = *(char **)ft_vec_get(str_vec, i);
		prefix_line = ft_strjoin(prefix, line);
		ft_putendl_fd(prefix_line, 1);
		free(prefix_line);
		i++;
	}
}

int	vec_remove_str(t_vec *src, size_t index)
{
	char	*s;

	if (!src || index >= src->len)
		return (0);
	s = *(char **)ft_vec_get(src, index);
	if (s)
		free(s);
	if (index < src->len - 1)
		ft_memmove(ft_vec_get(src, index), ft_vec_get(src, index + 1), (src->len
				- index - 1) * src->elem_size);
	src->len--;
	return (1);
}

size_t	get_str_index(t_vec *src, char *s)
{
	size_t	i;
	size_t	len;
	char	*entry;

	len = ft_strlen(s);
	i = 0;
	while (i < src->len)
	{
		entry = *(char **)ft_vec_get(src, i);
		if (entry && ft_strncmp(entry, s, len) == 0 && (entry[len] == '='
				|| entry[len] == '\0'))
			return (i);
		i++;
	}
	return (0);
}
