/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_redirection.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 14:14:03 by timurray          #+#    #+#             */
/*   Updated: 2025/11/30 17:35:19 by timurray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_builtin_input(t_cmd *cmd)
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
	return (EXIT_SUCCESS);
}

static int	handle_builtin_output(t_cmd *cmd)
{
	int	fd;

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

int	handle_builtin_redirections(t_cmd *cmd)
{
	if (handle_builtin_input(cmd) != EXIT_SUCCESS)
		return (EXIT_FAILURE);
	if (handle_builtin_output(cmd) != EXIT_SUCCESS)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
