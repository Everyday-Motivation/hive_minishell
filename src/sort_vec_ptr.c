/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_vec_ptr.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 17:49:12 by timurray          #+#    #+#             */
/*   Updated: 2025/10/05 18:28:22 by timurray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	swap_vec_str_ptr(t_vec *v, size_t index_a, size_t index_b)
{
	char	*temp;
	char	**line;
	char	**next_line;

	line = (char **)ft_vec_get(v, index_a);
	next_line = (char **)ft_vec_get(v, index_b);
	if (ft_strncmp(*line, *next_line, ft_strlen(*line)) > 0)
	{
		temp = *line;
		*line = *next_line;
		*next_line = temp;
	}
	return (1);
}

int	sort_vec_str_ptr(t_vec *v)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (i + 1 < v->len)
	{
		j = i + 1;
		while (j < v->len)
		{
			if (!swap_vec_str_ptr(v, i, j))
				return (0);
			j++;
		}
		i++;
	}
	return (1);
}
