/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_builtin.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 11:28:04 by timurray          #+#    #+#             */
/*   Updated: 2025/10/01 13:22:07 by timurray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	bi_unset(char **av, t_vec *env)
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
		while (j < env->len)
		{
			line = *(char **)ft_vec_get(env, j);
			if (line && (ft_strncmp(av[i], line, key_len) == 0)
				&& line[key_len] == '=')
			{
				vec_remove_str(env, j);
				continue ;
			}
			j++;
		}
		i++;
	}
	return (1);
}

/*
	TESTING
	-----------

	TODO: _ reject this

	print_str_vec(env);
	printf("\n----------------\n");
	char *t[] = {"PWD", "TERM", "HOME",  "SHLVL", NULL};
	bi_unset(t, env);
	print_str_vec(env);

 */