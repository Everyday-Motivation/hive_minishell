/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 11:26:49 by timurray          #+#    #+#             */
/*   Updated: 2025/11/19 12:56:30 by timurray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	bi_pwd(char **av, t_vec *env)
{
	char	*pwd;

	(void)env;
	if(av[0] != NULL)
	{
		ft_putstr_fd("pwd: too many arguments\n", 2);
		return (EXIT_FAILURE);
	}
	pwd = getcwd(NULL, 0);
	if (pwd == NULL)
	{
		perror("pwd error.");
		return (EXIT_FAILURE);
	}
	printf("%s\n", pwd);
	free(pwd);
	return (EXIT_SUCCESS);
}
