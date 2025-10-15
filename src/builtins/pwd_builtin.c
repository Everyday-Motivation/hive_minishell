/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 11:26:49 by timurray          #+#    #+#             */
/*   Updated: 2025/10/07 16:35:04 by timurray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"


// int	pwd_builtin(char **av, t_vec *env)
// {
// 	char	*pwd;

// 	pwd = getcwd(NULL, 0);
// 	if (pwd == NULL)
// 	{
// 		perror("pwd error.");
// 		return (EXIT_FAILURE);
// 	}
// 	printf("%s\n", pwd);
// 	free(pwd);
// 	return (0);
// }

/* 
pwd -L, 
pwd -P 
*/