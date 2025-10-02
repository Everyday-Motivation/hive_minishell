/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 16:31:48 by jaeklee           #+#    #+#             */
/*   Updated: 2025/10/02 10:57:41 by timurray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	init_env(t_vec *env, char **envp)
{
	if (!copy_env(env, envp))
		return (0);
	if (!str_in_str_vec(env, "PWD="))
		if (!add_pwd(env))
			return (0);
	if (!increment_shlvl(env))
		return (0);
	char *av[] = {
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
		"varname1111=valid",
		"PWD=newtest",
		"PWD=test  extras",
		NULL};
	bi_export(av, env);
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

int	increment_shlvl(t_vec *env)
{
	size_t	i;
	char	**entry;
	char	*new_level;
	char	*new_entry;

	i = 0;
	while (i < env->len)
	{
		entry = (char **)ft_vec_get(env, i);
		if (ft_strncmp(*entry, "SHLVL=", 6) == 0)
		{
			new_level = ft_itoa(ft_atoi(*entry + 6) + 1);
			if (!new_level)
				return (0);
			new_entry = ft_strjoin("SHLVL=", new_level);
			free(new_level);
			if (!new_entry)
				return (0);
			free(*entry);
			*entry = new_entry;
			return (1);
		}
		i++;
	}
	return (0);
}
