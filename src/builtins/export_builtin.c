/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_builtin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 11:27:50 by timurray          #+#    #+#             */
/*   Updated: 2025/12/12 11:11:55 by timurray         ###   ########.fr       */
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

static void	print_equal(char *line, char *equal_sign, char *prefix)
{
	char	*key;
	char	*value;

	key = ft_substr(line, 0, equal_sign - line);
	if (!key)
		return ;
	value = ft_strdup(equal_sign + 1);
	if (!value)
	{
		free(key);
		return ;
	}
	ft_putstr_fd(prefix, 1);
	ft_putstr_fd(key, 1);
	ft_putstr_fd("=\"", 1);
	ft_putstr_fd(value, 1);
	ft_putendl_fd("\"", 1);
	free(key);
	free(value);
}

static void	print_export_vec(t_vec *str_vec, char *prefix)
{
	size_t	i;
	char	*line;
	char	*equal_sign;

	i = 0;
	while (i < str_vec->len)
	{
		line = *(char **)ft_vec_get(str_vec, i);
		if (line[0] == '_' && line[1] == '=')
		{
			i++;
			continue ;
		}
		equal_sign = ft_strchr(line, '=');
		if (equal_sign)
			print_equal(line, equal_sign, prefix);
		else
		{
			ft_putstr_fd(prefix, 1);
			ft_putendl_fd(line, 1);
		}
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
	int		result;

	result = EXIT_SUCCESS;
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
			put_e("minishell: export: `", av[i], "`: not a valid identifier");
			result = EXIT_FAILURE;
		}
		i++;
	}
	return (result);
}
