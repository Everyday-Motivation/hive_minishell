/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaeklee <jaeklee@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 10:23:07 by timurray          #+#    #+#             */
/*   Updated: 2025/09/14 14:54:03 by jaeklee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <stdlib.h>

  char **copy_env(char **envp)
  {
	char **env;
	int len = 0;
	int i = 0;
		
	while(*env != '\0')
		len++;
	env = malloc((len + 1) * sizeof(char *));
	if (!env)
		return NULL;
	while (i < len)
	{
		env = ft_strdup(i);
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

int init_env(char **env, char **envp)
{
	*env = copy_env(envp);
	if (!*env)
		return (-1);
	return (0);
}

int	main(int ac, char **av, char **envp)
{
	(void)ac;
	(void)av;
	char **env;

	env = copy_env(envp);
	if (init_env(&env, envp) < 0)
	{
		perror("Environment init failed");
		return (1);
	}
	/// signal init
	// get input -> check if it is valid input 
	// -> if yes ->parsing  ->>> first tokenizing 
	
	
	return (0);
}
