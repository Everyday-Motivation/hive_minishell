/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_input.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 16:55:13 by jaeklee           #+#    #+#             */
/*   Updated: 2025/11/02 15:23:40 by timurray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_input(t_arena *arena, char **input)
{
	char	*line;
	size_t	len;

	if (isatty(STDIN_FILENO))
		line = readline("minishell$ ");
	else
		line = get_next_line(STDIN_FILENO);
	if (!line)
	{
		*input = NULL;
		return (0);
	}
	len = ft_strlen(line);
	if (len > 0 && line[len - 1] == '\n')
		line[len - 1] = '\0';
	*input = arena_strdup(arena, line, ft_strlen(line));
	if (!(*input))
	{
		if (isatty(STDIN_FILENO))
			free(line);
		return (0);
	}
	if (isatty(STDIN_FILENO))
		free(line);
	return (1);
}

int	check_input(t_arena *arena, char **input)
{
	int	len;

	if (!get_input(arena, input))
	{
		perror("get_input error");
		return (0);
	}
	len = ft_strlen(*input);
	if (len > 1024 || len <= 0)
	{
		perror("more_than Max input or allocation failed");
		return (0);
	}
	add_history(*input);
	return (1);
}
