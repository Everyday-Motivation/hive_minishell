/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shlvl.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 12:29:58 by timurray          #+#    #+#             */
/*   Updated: 2025/12/12 12:35:27 by timurray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*new_shlvl(int lvl)
{
	char	*num;
	char	*entry;

	if (lvl < 0)
		lvl = 0;
	else if (lvl > 999)
	{
		printf("minishell: warning: shell level (%i) too high "
			"resetting to 1\n",
			lvl);
		lvl = 1;
	}
	num = ft_itoa(lvl);
	if (!num)
		return (NULL);
	entry = ft_strjoin("SHLVL=", num);
	free(num);
	return (entry);
}

int	increment_shlvl(t_vec *env)
{
	size_t	i;
	char	**entry;
	char	*new_entry;

	i = 0;
	while (i < env->len)
	{
		entry = (char **)ft_vec_get(env, i);
		if (ft_strncmp(*entry, "SHLVL=", 6) == 0)
		{
			new_entry = new_shlvl(ft_atoi(*entry + 6) + 1);
			if (!new_entry)
				return (0);
			free(*entry);
			*entry = new_entry;
			return (1);
		}
		i++;
	}
	new_entry = ft_strdup("SHLVL=1");
	if (!new_entry)
		return (0);
	if (ft_vec_push(env, &new_entry) < 0)
		return (free(new_entry), 0);
	return (1);
}
