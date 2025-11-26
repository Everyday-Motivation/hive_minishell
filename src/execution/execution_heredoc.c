/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_heredoc.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 17:19:40 by timurray          #+#    #+#             */
/*   Updated: 2025/11/26 17:19:52 by timurray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	process_heredoc_str(t_cmd *cmd)
{
	int		pipefd[2];
	size_t	len;

	len = ft_strlen(cmd->heredoc_str);
	if (pipe(pipefd) == -1)
	{
		perror("pipe heredoc issue");
		return (1);
	}
	if (write(pipefd[WRITE_END], cmd->heredoc_str, len) == -1)
	{
		perror("heredoc write");
		close_pipes(pipefd);
		return (1);
	}
	if (dup2(pipefd[READ_END], STDIN_FILENO) == -1)
	{
		perror("dup2 to STDIN heredoc");
		close_pipes(pipefd);
		return (1);
	}
	free(cmd->heredoc_str);
	cmd->heredoc_str = NULL;
	close_pipes(pipefd);
	return (0);
}