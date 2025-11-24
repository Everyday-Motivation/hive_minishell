/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 11:27:06 by timurray          #+#    #+#             */
/*   Updated: 2025/11/24 13:29:58 by timurray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_address(char **av, t_vec *env)
{
	char	*address;

	if (!av[0] || ft_strcmp(av[0], "~") == 0)
	{
		address = get_env_value(env, "HOME");
		if (!address)
		{
			ft_putendl_fd("minishell: cd: HOME not set", 2);
			return (NULL);
		}
	}
	else if (ft_strcmp(av[0], "-") == 0)
	{
		address = get_env_value(env, "OLDPWD");
		printf("%s\n", address);
		if (!address)
		{
			ft_putendl_fd("minishell: cd: OLDPWD not set", 2);
			return (NULL);
		}
	}
	else
		address = av[0];
	return (address);
}

static int	update_pwd(t_info *info, char *key, char *pwd)
{
	char	*update;

	update = ft_strjoin(key, pwd);
	if (!update)
	{
		free(pwd);
		return (EXIT_FAILURE);
	}
	bi_export((char *[]){update, NULL}, info);
	free(update);
	free(pwd);
	return (EXIT_SUCCESS);
}

static int	get_pwd(char **pwd)
{
	*pwd = NULL;
	*pwd = getcwd(NULL, 0);
	if (!*pwd)
	{
		perror("getcwd error");
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	bi_cd(char **av, t_info *info)
{
	char	*address;
	char	*oldpwd;
	char	*newpwd;

	if (av[1])
	{
		ft_putendl_fd("minishell: cd: too many arguments", 2);
		return (EXIT_FAILURE);
	}
	if (get_pwd(&oldpwd) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	address = get_address(av, info->env);
	if (chdir(address) == -1)
	{
		perror("minishell: cd");
		free(oldpwd);
		return (EXIT_FAILURE);
	}
	if (update_pwd(info, "OLDPWD=", oldpwd) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (get_pwd(&newpwd) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (update_pwd(info, "PWD=", newpwd) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
