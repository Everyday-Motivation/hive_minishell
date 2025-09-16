/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 17:56:52 by timurray          #+#    #+#             */
/*   Updated: 2025/09/16 19:48:34 by timurray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static char	*get_prompt(void)
{
	char	*cwd;
	char	*prompt;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		return ("$ ");
	prompt = ft_strjoin(cwd, "$ ");
	if (!prompt)
	{
		free(cwd);
		return ("$ ");
	}
	free(cwd);
	return (prompt);
}

char	*read_line(int interactive)
{
	char	*prompt;
	char	*line;
	size_t	len;

	if (interactive)
	{
		prompt = get_prompt();
		line = readline(prompt);
		free(prompt);
		if (!line)
			exit_clear_rl_history();
		if (*line)
			add_history(line);
		return (line);
	}
	else
	{
		line = get_next_line(STDIN_FILENO);
		if (!line)
			return (NULL);
		len = ft_strlen(line);
		if (len && line[len - 1] == '\n')
			line[len - 1] = '\0';
		return (line);
	}
}
