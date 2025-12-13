/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_redirection.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 14:14:03 by timurray          #+#    #+#             */
/*   Updated: 2025/12/13 13:37:11 by timurray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	builtin_input(t_redir *redir)
{
	int	fd;

	fd = open(redir->data, O_RDONLY);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(redir->data);
		return (EXIT_FAILURE);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("minishell: duplicate input to STDIN failure");
		close(fd);
		return (EXIT_FAILURE);
	}
	close(fd);
	return (EXIT_SUCCESS);
}

static int	builtin_output(t_redir *redir)
{
	int	fd;
	int	flags;

	flags = O_WRONLY | O_CREAT;
	if (redir->type == D_GT)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	fd = open(redir->data, flags, 0644);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(redir->data);
		return (EXIT_FAILURE);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("minishell: duplicate output to STDOUT failure");
		close(fd);
		return (EXIT_FAILURE);
	}
	close(fd);
	return (EXIT_SUCCESS);
}

int	handle_builtin_redirections(t_cmd *cmd)
{
	size_t	i;
	t_redir	*redir;

	i = 0;
	while (i < cmd->redirs.len)
	{
		redir = ft_vec_get(&cmd->redirs, i);
		if (redir->type == S_LT)
		{
			if (builtin_input(redir) != EXIT_SUCCESS)
				return (EXIT_FAILURE);
		}
		else if (redir->type == D_LT)
		{
			if (redir->data && process_heredoc_str(&redir->data) != 0)
				return (EXIT_FAILURE);
		}
		else if (redir->type == S_GT || redir->type == D_GT)
		{
			if (builtin_output(redir) != EXIT_SUCCESS)
				return (EXIT_FAILURE);
		}
		i++;
	}
	return (EXIT_SUCCESS);
}
