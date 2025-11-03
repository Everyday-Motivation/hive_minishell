/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_helper.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 12:58:29 by timurray          #+#    #+#             */
/*   Updated: 2025/11/02 19:02:01 by timurray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_pipes(int pipefd[2])
{
	close(pipefd[READ_END]);
	close(pipefd[WRITE_END]);
}

void	process_heredoc_str(t_cmd *cmd)
{
	int		pipefd[2];
	char	*heredoc_str;
	size_t	len;

	heredoc_str = cmd->heredoc_str;
	len = ft_strlen(heredoc_str);
	if (pipe(pipefd) == -1)
	{
		perror("pipe heredoc issue");
		exit(1);
	}
	if (write(pipefd[WRITE_END], heredoc_str, len) == -1)
	{
		perror("heredoc write");
		close_pipes(pipefd);
		exit(1);
	}
	if (dup2(pipefd[READ_END], STDIN_FILENO) == -1)
	{
		perror("dup2 to STDIN heredoc");
		close_pipes(pipefd);
		exit(1);
	}
	close_pipes(pipefd);
}
