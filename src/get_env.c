/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaeklee <jaeklee@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 16:31:48 by jaeklee           #+#    #+#             */
/*   Updated: 2025/09/15 15:43:11 by jaeklee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"



int init_env(t_vec *env, char **envp)
{
    if (!copy_env(env, envp))
        return 0;

    if (!pwd_exists(env))
        if (!add_pwd(env))
            return 0;

    if (!increment_shlvl(env))
        return 0;

    return 1;
}

int copy_env(t_vec *env, char **envp)
{
	int i;
	int j;
	char *dup;

	i = 0;
    if (ft_vec_new(env, 0, sizeof(char *)) < 0)
        return 0;
    while (envp[i])
    {
        dup = strdup(envp[i]);
        if (!dup)
        {
            j = 0;
            while (j < (int)env->len)
            {
                free(*(char **)ft_vec_get(env, j));
                j++;
            }
            ft_vec_free(env);
            return 0;
        }
        if (ft_vec_push(env, &dup) < 0)
        {
            free(dup);
            return (0);
        }
        i++;
    }
    return (1);
}

int pwd_exists(t_vec *env)
{
    size_t i;
	char *entry;
	
	i = 0;
    while (i < env->len)
    {
        entry = *(char **)ft_vec_get(env, i);
        if (strncmp(entry, "PWD=", 4) == 0)
            return 1;
        i++;
    }
    return 0;
}

int add_pwd(t_vec *env)
{
    char *cwd;
    char *pwd_tmp;

    cwd = getcwd(NULL, 0);
    if (!cwd)
        return 0;
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

int increment_shlvl(t_vec *env)
{
    size_t i;
	int level;
	char **entry;
	char *new_level;
	char *new_entry;
	
	i = 0;
    while (i < env->len)
    {
        entry = ft_vec_get(env, i);
        if (strncmp(*entry, "SHLVL=", 6) == 0)
        {
            level = ft_atoi(*entry + 6); 
            new_level = ft_itoa(level + 1);
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
    return (1);
}



// char **copy_env(char **envp)
// {
// 	char **env;
// 	int len = 0;
// 	int i = 0;
		
// 	while(envp[len])
// 	{
// 		printf("obj: %s\n len %i\n",envp[len], len);
// 		len++;
// 	}
// 	env = malloc((len + 1) * sizeof(char *));
// 	if (!env)
// 		return NULL;
// 	while (i < len)
// 	{
// 		env[i] = ft_strdup(envp[i]);
// 		if (!env[i])
// 		{
// 			while (--i >= 0)
// 				free(env[i]);
// 			free(env);
// 			return (NULL);
// 		}
// 		i++;
// 	}
// 	env[i] = NULL;
// 	return env;
// }

// int pwd_exists(char **env)
// {
// 	int i = 0;
// 	while (env[i])
// 	{
// 		if (strncmp(env[i], "PWD=", 4) == 0)
// 			return 1;
// 		i++;
// 	}
// 	return 0;
// }

// char **add_pwd(char **env)
// {
// 	int		len = 0;
// 	char	*cwd;
// 	char	*pwd_entry;
// 	char	**new_env;
// 	int		i = 0;

// 	cwd = getcwd(NULL, 0);
// 	if (!cwd)
// 		return (env);
// 	while (env[len])
// 		len++;
// 	new_env = (char **)ft_calloc(len + 2, sizeof(char *));
// 	if (!new_env)
// 	{
// 		free(cwd);
// 		return (env);
// 	}
// 	while (i < len)
// 	{
// 		new_env[i] = ft_strdup(env[i]);
// 		if (!new_env[i])
// 		{
// 			free(cwd);
// 			while (--i >= 0)
// 				free(new_env[i]);
// 			free(new_env);
// 			return (env);
// 		}
// 		i++;
// 	}
// 	pwd_entry = ft_strjoin("PWD=", cwd);
// 	if (!pwd_entry)
// 	{
// 		free(cwd);
// 		while (--i >= 0)
// 			free(new_env[i]);
// 		free(new_env);
// 		return (env);
// 	}
// 	new_env[i++] = pwd_entry;
// 	new_env[i] = NULL;
// 	free(cwd);
// 	i = 0;
// 	while (env[i])
// 		free(env[i++]);
// 	free(env);

// 	return (new_env);
// }

// int	increment_shlvl(char **env)
// {
// 	int		i;
// 	int		level;
// 	char	*new_value;
// 	char	*tmp;

// 	i = 0;
// 	while (env[i])
// 	{
// 		if (ft_strncmp(env[i], "SHLVL=", 6) == 0)
// 		{
// 			level = ft_atoi(env[i] + 6);
// 			tmp = ft_itoa(level + 1);
// 			if (!tmp)
// 				return (0);
// 			new_value = ft_strjoin("SHLVL=", tmp);
// 			free(tmp);
// 			if (!new_value)
// 				return (0);
// 			free(env[i]);
// 			env[i] = new_value;
// 			return (1);
// 		}
// 		i++;
// 	}
// 	return (1);
// }

// int init_env(char ***env, char **envp)
// {
// 	*env = copy_env(envp);
// 	if (!*env)
// 		return 0;
// 	if (!pwd_exists(*env))
// 		*env = add_pwd(*env);
// 	if (!pwd_exists(*env))
// 	{
// 		perror("PWD allocation failed \n");
// 		return (0);
// 	}
// 	if (!increment_shlvl(*env))
// 	{
// 		perror("shlvl allocation failed \n");
// 		return (0);
// 	}
// 	return 1;
// }

