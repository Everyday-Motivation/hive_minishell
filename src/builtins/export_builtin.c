/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_builtin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 11:27:50 by timurray          #+#    #+#             */
/*   Updated: 2025/12/02 10:10:03 by timurray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_valid_export(char *line)
{
	size_t	i;

	if (!line || !((ft_isalpha(line[0])) || line[0] == '_'))
		return (0);
	i = 1;
	while (line[i] && line[i] != '=')
	{
		if (!(ft_isalnum(line[i]) || line[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

static void	print_export_vec(t_vec *str_vec, char *prefix)
{
	size_t	i;
	char	*line;
	char	*prefix_line;

	i = 0;
	while (i < str_vec->len)
	{
		line = *(char **)ft_vec_get(str_vec, i);
		if (line[0] == '_' && line[1] == '=')
		{
			i++;
			continue ;
		}
		prefix_line = ft_strjoin(prefix, line);
		ft_putendl_fd(prefix_line, 1);
		free(prefix_line);
		i++;
	}
}

static int	export_no_args(t_vec *env)
{
	t_vec	env_cpy;

	ft_vec_new(&env_cpy, 0, sizeof(char *));
	if (!copy_vec_str_ptr(&env_cpy, env))
		return (0);
	if (!sort_vec_str_ptr(&env_cpy))
		return (0);
	print_export_vec(&env_cpy, "declare -x ");
	ft_vec_free(&env_cpy);
	return (1);
}

int	bi_export(char **av, t_info *info)
{
	size_t	i;

	if (!av || !av[0])
	{
		if (!export_no_args(info->env))
			return (0);
	}
	i = 0;
	while (av[i])
	{
		if (is_valid_export(av[i]))
		{
			if (!env_add_update_line(info->env, av[i]))
				return (EXIT_FAILURE);
		}
		else
		{
			ft_putstr_fd("minishell: export: `", 2);
			ft_putstr_fd(av[i], 2);
			ft_putendl_fd("`: not a valid identifier", 2);
			return (EXIT_FAILURE);
		}
		i++;
	}
	return (EXIT_SUCCESS);
}
