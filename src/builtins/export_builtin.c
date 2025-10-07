/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_builtin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 11:27:50 by timurray          #+#    #+#             */
/*   Updated: 2025/10/05 18:30:49 by timurray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	is_valid_export(char *line)
{
	size_t	i;
	size_t	len;
	int		has_equals;

	if (!line || !((ft_isalpha(line[0])) || line[0] == '_'))
		return (0);
	i = 0;
	has_equals = 0;
	len = ft_strlen(line);
	while ((i < len))
	{
		if (!((ft_isalnum(line[i])) || line[i] == '=' || line[i] == '_'
				|| line[i] == ' '))
			return (0);
		if (!has_equals && ft_isspace(line[i]))
			return (0);
		if (line[i] == '=')
			has_equals = 1;
		i++;
	}
	if (has_equals)
		return (1);
	return (0);
}

static int	export_no_args(t_vec *env)
{
	t_vec	env_cpy;

	ft_vec_new(&env_cpy, 0, sizeof(char *));
	if (!copy_vec_str_ptr(&env_cpy, env))
		return (0);
	if (!sort_vec_str_ptr(&env_cpy))
		return (0);
	print_str_vec(&env_cpy, "declare -x ");
	ft_vec_free(&env_cpy);
	return (1);
}

int	bi_export(char **av, t_vec *env)
{
	size_t	i;

	if (!av || !av[0])
	{
		if (!export_no_args(env))
			return (0);
	}
	i = 0;
	while (av[i])
	{
		if (is_valid_export(av[i]))
		{
			if (!env_add_update_line(env, av[i]))
				return (0);
		}
		else
			ft_printf("minishell: export: `%s`: not a valid identifier\n",
				av[i]);
		i++;
	}
	return (1);
}

/*

TODO: Remove

TEST CASES
sneak into init_env

	char *ava[] = {
		"7asd=invalid",
		"te,st=invalid",
		"_=valid", 
		"te-est=invalid",
		"test",
		"hey=hello",
		"hey=hej",
		"nihao=hey",
		"hey=nihao",
		"PWD=test",
		"KeyNOval=",
		"KeyNOvalQ=",
		"varname1111=valid",
		"PWD=newtest",
		"PWD=test  extras",
		"NOSPACE=testextras",
		"SPACES=test extras",
		NULL};
	bi_export(ava, env);
	char *av[] = {NULL};
	bi_export(av, env);
 */