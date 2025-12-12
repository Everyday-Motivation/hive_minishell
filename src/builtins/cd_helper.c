/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_helper.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 11:16:41 by timurray          #+#    #+#             */
/*   Updated: 2025/12/12 11:19:17 by timurray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	change_dir(char *address, char *oldpwd)
{
	if (chdir(address) == -1)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		perror(address);
		free(oldpwd);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
