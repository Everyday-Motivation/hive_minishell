/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 16:31:48 by jaeklee           #+#    #+#             */
/*   Updated: 2025/12/12 12:30:24 by timurray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	init_env(t_vec *env, char **envp)
{
	if (envp[0] == NULL)
	{
		if (fix_empty_env(env) == EXIT_FAILURE)
			return (0);
		return (1);
	}
	else
	{
		if (!copy_env(env, envp))
			return (0);
		if (!str_in_str_vec(env, "PWD"))
			if (!add_pwd(env))
				return (0);
		if (!increment_shlvl(env))
			return (0);
	}
	return (1);
}

int	copy_env(t_vec *env, char **envp)
{
	size_t	i;
	char	*dup;

	i = 0;
	if (ft_vec_new(env, 0, sizeof(char *)) < 0)
		return (0);
	while (envp[i])
	{
		dup = ft_strdup(envp[i]);
		if (!dup)
		{
			free_str_vec(env);
			return (0);
		}
		if (ft_vec_push(env, &dup) < 0)
		{
			free(dup);
			free_str_vec(env);
			return (0);
		}
		i++;
	}
	return (1);
}

int	add_pwd(t_vec *env)
{
	char	*cwd;
	char	*pwd_tmp;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (0);
	pwd_tmp = ft_strjoin("PWD=", cwd);
	free(cwd);
	if (!pwd_tmp)
		return (0);
	if (ft_vec_push(env, &pwd_tmp) < 0)
	{
		free(pwd_tmp);
		return (0);
	}
	return (1);
}

char	*get_env_value(t_vec *env, const char *key)
{
	size_t	i;
	size_t	name_len;
	char	*entry;

	i = 0;
	if (!key || key[0] == '\0')
		return (NULL);
	name_len = ft_strlen(key);
	while (i < env->len)
	{
		entry = *(char **)ft_vec_get(env, i);
		if (entry && strncmp(entry, key, name_len) == 0
			&& entry[name_len] == '=')
			return (entry + name_len + 1);
		i++;
	}
	return (NULL);
}
