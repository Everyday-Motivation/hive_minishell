/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 17:56:52 by timurray          #+#    #+#             */
/*   Updated: 2025/09/16 19:04:29 by timurray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char *get_prompt(void)
{
	char	*cwd;
	char	*prompt;

	cwd = getcwd(NULL, 0);
	if(!cwd)
		return ("$ ");
	prompt = ft_strjoin(cwd, "$ ");
	if(!prompt)
	{
		free(cwd);
		return ("$ ");
	}
	free(cwd);
	return (prompt);
}
