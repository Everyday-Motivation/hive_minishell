/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 11:28:16 by timurray          #+#    #+#             */
/*   Updated: 2025/10/07 17:01:56 by timurray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static size_t	key_len(char *line)
{
	size_t	i;

	i = 0;
	while (line[i] != '=')
		i++;
	return (i);
}

int	bi_env(char **av, t_vec *env)
{
	size_t	i_env;
	size_t	i_av;
	char	*line;
	int		overwrite;
	int		found;

	if (!env || env->len == 0)
		return (0);
	i_env = 0;
	while (i_env < env->len)
	{
		overwrite = 0;
		line = *(char **)ft_vec_get(env, i_env);
		i_av = 0;
		while (av[i_av])
		{
			//valid key?
			if (ft_strncmp(line, av[i_av], key_len(line)) == 0)
			{
				overwrite = 1;
				break ;
			}
			i_av++;
		}
		if (overwrite)
			ft_putendl_fd(av[i_av], 1);
		else
			ft_putendl_fd(line, 1);
		i_env++;
	}
	i_av = 0;
	while (av[i_av])
	{
		//valid key
		found = 0;
		i_env = 0;
		while (i_env < env->len)
		{
			line = *(char **)ft_vec_get(env, i_env);
			if (ft_strncmp(line, av[i_av], key_len(line)) == 0)
			{
				found = 1;
				break ;
			}
			i_env++;
		}
		if (!found)
			ft_putendl_fd(av[i_av], 1);
		i_av++;
	}
	return (1);
}

/*
TODO: Doesn't show variables without var=
TODO: Arguments passed are added to print out but not added to env. Same names get overwritten for the print out but not permanently.

env -i bash to start empty
 */