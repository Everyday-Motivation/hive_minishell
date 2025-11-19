/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 11:27:06 by timurray          #+#    #+#             */
/*   Updated: 2025/11/19 22:08:14 by timurray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	bi_cd(char **av, t_vec *env)
{
	int result;
	char *cur_dir;
	char *address;

	(void)env;
	cur_dir = getcwd(NULL, 0);
	if (cur_dir == NULL)
	{
		perror("getcwd error");
		return (0);
	}
	if(av[1])
	{
		perror("minishell: cd: too many arguments");
		return (EXIT_FAILURE);
	}	
	if (!av[0])
	{
		address = get_env_value(env, "HOME");
		ft_printf("address: %s\n", address);
		//TODO go $HOME
	}
	else
	{
		result = chdir(av[0]);
		if(result != 0)
		{
			perror("minishell: cd: No such file or directory");
			return (0);
		}
		else
		{
			char *s;
			s = ft_strjoin("OLDPWD=", cur_dir);
			bi_export((char *[]){ s, NULL }, env);
			free(cur_dir);
			cur_dir = getcwd(NULL, 0);
			if (cur_dir == NULL)
			{
				perror("getcwd error");
				return (0);
			}
			free(s);
			s = ft_strjoin("PWD=",cur_dir);
			bi_export((char *[]){ s, NULL }, env);
			free(cur_dir);
		}
	}
	return (1);
}


/* 
TODO: no args, goes to $HOME
TODO: - , ~ think about switch, home, root
TODO: too many arguments
*/
