/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 16:40:59 by timurray          #+#    #+#             */
/*   Updated: 2025/10/12 10:28:57 by timurray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int executor(void)
{
	pid_t pid;

	pid = fork();
	if(pid == 0)
	{
		
		printf("hey it's 0\n");
	}
	else
	{

		printf("hey it's %d\n", pid);
	}
	return (1);
}
