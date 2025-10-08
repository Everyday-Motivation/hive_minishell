/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 11:28:16 by timurray          #+#    #+#             */
/*   Updated: 2025/10/08 12:51:52 by timurray         ###   ########.fr       */
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

int valid_env_key(char *line)
{
	size_t i;

	i = 0;
	while(line[i])
	{
		if(line[i] == '=')
			return (1);
		i++;
	}
	return (0);
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
			if(!valid_env_key(av[i_av]))
			{
				printf("env: ‘%s’: No such file or directory\n", av[i_av]);
				return (0);
			}
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
TODO: Doesn't show variables without var= such as var which throws error.

env -i bash to start empty
 */