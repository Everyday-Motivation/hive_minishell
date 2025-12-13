/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_heredoc.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 17:19:40 by timurray          #+#    #+#             */
/*   Updated: 2025/12/13 13:30:14 by timurray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	process_heredoc_str(char **str)
{
	int		pipefd[2];
	size_t	len;

	len = ft_strlen(*str);
	if (pipe(pipefd) == -1)
	{
		perror("minishell: heredoc pipe creation failure");
		return (1);
	}
	if (write(pipefd[WRITE_END], *str, len) == -1)
	{
		perror("minishell: heredoc write to pipe failure");
		close_pipes(pipefd);
		return (1);
	}
	if (dup2(pipefd[READ_END], STDIN_FILENO) == -1)
	{
		perror("minishell: heredoc duplicate STDIN failure");
		close_pipes(pipefd);
		return (1);
	}
	free(*str);
	*str = NULL;
	close_pipes(pipefd);
	return (0);
}

//TODO: test large heredoc