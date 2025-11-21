/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 11:27:06 by timurray          #+#    #+#             */
/*   Updated: 2025/11/21 19:18:32 by timurray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	bi_cd(char **av, t_info *info)
{
	char *address;
	char *oldpwd;
	char *newpwd;
	char *update;


	if(av[1])
	{
		ft_putendl_fd("minishell: cd: too many arguments", 2);
		return (EXIT_FAILURE);
	}	
	
	oldpwd = getcwd(NULL, 0);
	if(!oldpwd)
	{
		perror("getcwd error");
		return (EXIT_FAILURE);
	}

	if (!av[0] || ft_strcmp(av[0], "~") == 0)
	{
		address = get_env_value(info->env, "HOME");
		if(!address)
		{
			ft_putendl_fd("minishell: cd: HOME not set", 2);
			free(oldpwd);
			return (EXIT_FAILURE);
		}
	}
	else if (ft_strcmp(av[0], "-") == 0)
	{
		address = get_env_value(info->env, "OLDPWD");
		if(!address)
		{
			ft_putendl_fd("minishell: cd: OLDPWD not set", 2);
			free(oldpwd);
			return (EXIT_FAILURE);
		}
	}
	else
		address = av[0];

	if (chdir(address) == -1)
	{
		perror("minishell: cd: No such file or directory");
		return (EXIT_FAILURE);
	}

	update = ft_strjoin("OLDPWD=", oldpwd);
	if (!update)
	{
		free(oldpwd);
		return(EXIT_FAILURE);
	}
	bi_export((char *[]){ update, NULL }, info);
	free(update);
	free(oldpwd);

	newpwd = getcwd(NULL, 0);
	if(!newpwd)
	{
		perror("new directory");
		return (EXIT_FAILURE);
	}
	update = ft_strjoin("PWD=", newpwd);
	if (!update)
	{
		free(newpwd);
		return(EXIT_FAILURE);
	}
	bi_export((char *[]){ update, NULL }, info);
	free(update);
	free(newpwd);

	return (EXIT_SUCCESS);
}


/* 

TODO: shrink

*/
