/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_str_helper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 10:45:57 by timurray          #+#    #+#             */
/*   Updated: 2025/09/28 19:21:46 by timurray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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

	if (!str_vec || !str)
		return (0);
	i = 0;
	while (i < str_vec->len)
	{
		line = *(char **)ft_vec_get(str_vec, i);
		if (ft_strncmp(line, str, ft_strlen(str)) == 0)
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
