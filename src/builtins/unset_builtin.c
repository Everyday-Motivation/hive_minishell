/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_builtin.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 11:28:04 by timurray          #+#    #+#             */
/*   Updated: 2025/11/21 19:12:13 by timurray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	bi_unset(char **av, t_info *info)
{
	size_t	i;
	size_t	j;
	size_t	key_len;
	char	*line;

	i = 0;
	while (av[i])
	{
		key_len = ft_strlen(av[i]);
		j = 0;
		while (j < info->env->len)
		{
			line = *(char **)ft_vec_get(info->env, j);
			if (line && (ft_strncmp(av[i], line, key_len) == 0)
				&& line[key_len] == '=')
			{
				vec_remove_str(info->env, j);
				continue ;
			}
			j++;
		}
		i++;
	}
	return (EXIT_SUCCESS);
}
