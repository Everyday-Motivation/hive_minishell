/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_builtin.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 11:28:04 by timurray          #+#    #+#             */
/*   Updated: 2025/12/03 12:01:43 by timurray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	process_unset(t_info *info, char *av, size_t key_len)
{
	size_t	j;
	char	*line;

	j = 0;
	while (j < info->env->len)
	{
		line = *(char **)ft_vec_get(info->env, j);
		if (line && (ft_strncmp(av, line, key_len) == 0)
			&& (line[key_len] == '=' || line[key_len] == '\0'))
		{
			vec_remove_str(info->env, j);
			continue ;
		}
		j++;
	}
}

int	bi_unset(char **av, t_info *info)
{
	size_t	i;
	size_t	key_len;

	i = 0;
	while (av[i])
	{
		key_len = ft_strlen(av[i]);
		if (key_len == 1 && av[i][0] == '_')
		{
			i++;
			continue ;
		}
		process_unset(info, av[i], key_len);
		i++;
	}
	return (EXIT_SUCCESS);
}
