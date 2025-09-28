/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_str_helper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 10:45:57 by timurray          #+#    #+#             */
/*   Updated: 2025/09/28 12:18:06 by timurray         ###   ########.fr       */
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
//TODO: remove if unused
void	print_str_vec(t_vec *str_vec)
{
	size_t	i;
	char	*line;

	i = 0;
	while (i < str_vec->len)
	{
		line = *(char **)ft_vec_get(str_vec, i);
		ft_putendl_fd(line, 1);
		i++;
	}
}
