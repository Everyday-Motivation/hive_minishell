/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 11:28:30 by timurray          #+#    #+#             */
/*   Updated: 2025/11/21 19:10:51 by timurray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


static void free_exit(t_vec *env, int code)
{
	ft_vec_free(env);
	// TODO: free stuff, no free stuff.
	// Or send signal?
	// add exit codes;
	exit(code);
}

static int is_all_num(char *av)
{
	size_t i;

	i = 0;
	while(av[i])
	{
		if(!ft_isdigit(av[i]))
			return (0);
		i++;
	}
	return (1);
}


int	bi_exit(char **av, t_info *info)
{
	ft_putendl_fd("exit", 1);
	if (!av || !av[0])
		free_exit(info->env, 0);
	if (av[1])
	{
		ft_putendl_fd("minishell: exit: too many arguments", 2);
		return (1);
	}
	if (av[0])
	{
		if(!is_all_num(av[0]))
		{
			ft_putstr_fd("minishell: exit: ", 2);
			ft_putstr_fd(av[0], 2);
			ft_putendl_fd(": numeric argument required", 2);
			free_exit(info->env, 2);
		}
	}
	
	free_exit(info->env, 0);
	return (EXIT_SUCCESS);
}

/*
TODO: Should clean up things.
TODO: Handle arguments (numeric, non-numeric, too many)
*/
