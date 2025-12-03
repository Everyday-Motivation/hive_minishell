/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_helper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 11:20:23 by timurray          #+#    #+#             */
/*   Updated: 2025/12/03 11:35:12 by timurray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	fix_empty_env(t_vec *env)
{
	char	*shlvl;
	char	*underscore;

	if (ft_vec_new(env, 0, sizeof(char *)) < 0)
		return (EXIT_FAILURE);
	if (!add_pwd(env))
		return (EXIT_FAILURE);
	shlvl = ft_strdup("SHLVL=1");
	if (!shlvl || ft_vec_push(env, &shlvl) < 0)
	{
		free(shlvl);
		return (EXIT_FAILURE);
	}
	underscore = ft_strdup("_=/usr/bin/env");
	if (!underscore || ft_vec_push(env, &underscore) < 0)
	{
		free(underscore);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
