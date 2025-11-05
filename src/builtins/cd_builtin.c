/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 11:27:06 by timurray          #+#    #+#             */
/*   Updated: 2025/11/05 15:01:20 by timurray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	bi_cd(char **av, t_vec *env)
{
	int result;

	(void)env;
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
			//set oldpwd in env
			//set pwd in env;
			// think about switch, home, root
		}

	}
	return (1);
}


/* 
TASKS
TODO: update PWD?
TODO: - , ~.
*/
