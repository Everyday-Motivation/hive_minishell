/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 11:26:49 by timurray          #+#    #+#             */
/*   Updated: 2025/11/21 19:10:10 by timurray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	bi_pwd(char **av, t_info *info)
{
	char	*pwd;

	(void)info;
	(void)av;
	pwd = getcwd(NULL, 0);
	if (pwd == NULL)
	{
		perror("pwd error.");
		return (EXIT_FAILURE);
	}
	ft_putendl_fd(pwd, 1);
	free(pwd);
	return (EXIT_SUCCESS);
}
