/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_helper.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 12:58:29 by timurray          #+#    #+#             */
/*   Updated: 2025/11/03 11:50:39 by timurray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_pipes(int pipefd[2])
{
	close(pipefd[READ_END]);
	close(pipefd[WRITE_END]);
}

int	process_heredoc_str(t_cmd *cmd)
{
	int		pipefd[2];
	char	*heredoc_str;
	size_t	len;

	heredoc_str = cmd->heredoc_str;
	len = ft_strlen(heredoc_str);
	if (pipe(pipefd) == -1)
	{
		perror("pipe heredoc issue");
		return (1);
	}
	if (write(pipefd[WRITE_END], heredoc_str, len) == -1)
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

int	handle_builtin_redirections(t_cmd *cmd)
{
	int	fd;

	if (cmd->input_file)
	{
		fd = open(cmd->input_file, O_RDONLY);
		if (fd == -1)
		{
			perror(cmd->input_file);
			return (EXIT_FAILURE);
		}
		if (dup2(fd, STDIN_FILENO) == -1)
		{
			perror("dup2 in file issue");
			close(fd);
			return (EXIT_FAILURE);
		}
		close(fd);
	}
	else if (cmd->heredoc_str)
	{
		if (process_heredoc_str(cmd) != 0)
			return (EXIT_FAILURE);
	}
	if (cmd->output_file)
	{
		if (cmd->append)
			fd = open(cmd->output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else
			fd = open(cmd->output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1)
		{
			perror("open outfile issue");
			return (EXIT_FAILURE);
		}
		if (dup2(fd, STDOUT_FILENO) == -1)
		{
			perror("dup2 out");
			close(fd);
			return (EXIT_FAILURE);
		}
		close(fd);
	}
	return (EXIT_SUCCESS);
}
