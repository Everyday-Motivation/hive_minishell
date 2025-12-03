/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 17:56:52 by timurray          #+#    #+#             */
/*   Updated: 2025/12/03 17:58:42 by timurray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_prompt(void)
{
	char	*cwd;
	char	*prompt;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (ft_strdup("$ "));
	prompt = ft_strjoin(cwd, "$ ");
	if (!prompt)
	{
		free(cwd);
		return (ft_strdup("$ "));
	}
	free(cwd);
	return (prompt);
}

char	*read_line_interactive(void)
{
	char	*prompt;
	char	*line;

	prompt = get_prompt();
	line = readline(prompt);
	free(prompt);
	if (!line)
	{
		exit_clear_rl_history();
		return (NULL);
	}
	if (*line)
		add_history(line);
	return (line);
}

char	*read_line_noninteractive(void)
{
	char	*line;
	size_t	len;

	line = get_next_line(STDIN_FILENO);
	if (!line)
		return (NULL);
	len = ft_strlen(line);
	if (len && line[len - 1] == '\n')
		line[len - 1] = '\0';
	return (line);
}

char	*read_line(int interactive)
{
	if (interactive)
		return (read_line_interactive());
	else
		return (read_line_noninteractive());
}
