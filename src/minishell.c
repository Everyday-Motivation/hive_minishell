/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaeklee <jaeklee@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 10:23:07 by timurray          #+#    #+#             */
/*   Updated: 2025/09/14 16:15:13 by jaeklee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

  char **copy_env(char **envp)
{
	char **env;
	int len = 0;
	int i = 0;
		
	while(envp[len])
	{
		printf("obj: %s\n len %i\n",envp[len], len);
		
		len++;
	}
	env = malloc((len + 1) * sizeof(char *));
	if (!env)
		return NULL;
	while (i < len)
	{
		env[i] = ft_strdup(envp[i]);
		if (!env[i])
		{
			while (--i >= 0)
				free(env[i]);
			free(env);
			return (NULL);
		}
		i++;
	}
	env[i] = NULL;
	return env;
}

int pwd_exists(char **env)
{
	int i = 0;
	while (env[i])
	{
		if (strncmp(env[i], "PWD=", 4) == 0)
			return 1;
		i++;
	}
	return 0;
}

char **add_pwd(char **env)
{
	int		len = 0;
	char	*cwd;
	char	*pwd_entry;
	char	**new_env;
	int		i = 0;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (env);

	while (env[len])
		len++;

	new_env = (char **)ft_calloc(len + 2, sizeof(char *));
	if (!new_env)
	{
		free(cwd);
		return (env);
	}
	while (i < len)
	{
		new_env[i] = ft_strdup(env[i]);
		if (!new_env[i])
		{
			free(cwd);
			while (--i >= 0)
				free(new_env[i]);
			free(new_env);
			return (env);
		}
		i++;
	}
	pwd_entry = ft_strjoin("PWD=", cwd);
	if (!pwd_entry)
	{
		free(cwd);
		while (--i >= 0)
			free(new_env[i]);
		free(new_env);
		return (env);
	}
	new_env[i++] = pwd_entry;
	new_env[i] = NULL;
	free(cwd);
	i = 0;
	while (env[i])
		free(env[i++]);
	free(env);

	return (new_env);
}

int init_env(char ***env, char **envp)
{
	*env = copy_env(envp);
	if (!*env)
		return 0;
	if (!pwd_exists(*env))
		*env = add_pwd(*env);
	if (!pwd_exists(*env))
	{
		perror("PWD allocation failed \n");
		return (0);
	}

	return 1;
}

int	main(int ac, char **av, char **envp)
{
	(void)ac;
	(void)av;
	char **env;

	if (!init_env(&env, envp))
	{
		perror("Environment init failed");
		return (EXIT_FAILURE);
	}
	
	/// signal init
	// get input -> check if it is valid input 
	// -> if yes ->parsing  ->>> first tokenizing 
	
	
	return (EXIT_SUCCESS);
}
