/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 19:11:27 by timurray          #+#    #+#             */
/*   Updated: 2025/09/14 19:19:55 by timurray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	mini_cd(char **args)
{
	if (!args[1])
		perror("Expected argument to cd");
	else if (chdir(args[1]) != 0)
		perror("minishell: cd: No such file or directory");
	return (1);
}
