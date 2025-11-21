/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 11:28:16 by timurray          #+#    #+#             */
/*   Updated: 2025/11/21 19:10:30 by timurray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t	key_len(char *line)
{
	size_t	i;

	i = 0;
	while (line[i] != '=')
		i++;
	return (i);
}

static int	valid_env_key(char *line)
{
	size_t	i;

	i = 0;
	while (line[i])
	{
		if (line[i] == '=')
			return (1);
		i++;
	}
	printf("env: ‘%s’: No such file or directory\n", line);
	return (0);
}

static int	exist_val_print(char **av, t_vec *env, int i_env)
{
	char	*line;
	int		new_value;
	size_t	i_av;

	new_value = 1;
	line = *(char **)ft_vec_get(env, i_env);
	i_av = 0;
	while (av[i_av])
	{
		if (!valid_env_key(av[i_av]))
			return (0);
		new_value = ft_strncmp(line, av[i_av], key_len(line));
		if (new_value == 0)
			break ;
		i_av++;
	}
	if (new_value == 0)
		ft_putendl_fd(av[i_av], 1);
	else
		ft_putendl_fd(line, 1);
	i_env++;
	return (1);
}

static int	new_val_print(char **av, t_vec *env, int i_av)
{
	char	*line;
	int		found;
	size_t	i_env;

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
	return (1);
}

int	bi_env(char **av, t_info *info)
{
	size_t	i_env;
	size_t	i_av;

	if (!info->env || info->env->len == 0)
		return (0);
	i_env = 0;
	while (i_env < info->env->len)
	{
		if (!exist_val_print(av, info->env, i_env))
			return (0);
		i_env++;
	}
	i_av = 0;
	while (av[i_av])
	{
		if (!new_val_print(av, info->env, i_av))
			return (0);
		i_av++;
	}
	return (1);
}
