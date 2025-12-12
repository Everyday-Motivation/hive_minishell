/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_to_arr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 16:44:13 by timurray          #+#    #+#             */
/*   Updated: 2025/12/12 16:53:18 by timurray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**vec_to_arr(t_vec *v)
{
	char	**arr;
	size_t	i;

	arr = malloc((v->len + 1) * sizeof(char *));
	if (!arr)
		return (NULL);
	i = 0;
	while (i < v->len)
	{
		arr[i] = *(char **)ft_vec_get(v, i);
		i++;
	}
	arr[i] = NULL;
	return (arr);
}
