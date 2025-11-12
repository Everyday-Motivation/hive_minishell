/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 11:27:06 by timurray          #+#    #+#             */
/*   Updated: 2025/11/12 14:34:25 by timurray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	bi_cd(char **av, t_vec *env)
{
	int result;
	char *cur_dir;

	(void)env;
	cur_dir = getcwd(NULL, 0);
	if (cur_dir == NULL)
	{
		perror("getcwd error");
		return (0);
	}	
	if (!av[0])
		perror("Expected argument to cd");
	else
	{
		result = chdir(av[1]);
		if(result != 0)
		{
			perror("minishell: cd: No such file or directory");
			return (0);
		}
		else
		{
			bi_export((char *[]){ "OLDPWD=", cur_dir, NULL }, env);
			free(cur_dir);
			cur_dir = getcwd(NULL, 0);
			if (cur_dir == NULL)
			{
				perror("getcwd error");
				return (0);
			}
			bi_export((char *[]){ "PWD=", cur_dir, NULL }, env);
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
