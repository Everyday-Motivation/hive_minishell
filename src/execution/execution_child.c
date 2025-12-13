/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_child.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 14:24:26 by timurray          #+#    #+#             */
/*   Updated: 2025/12/13 13:30:06 by timurray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	child_input(t_cmd *cmd, t_redir *redir, t_vec *cmds, int pipefd[3])
{
	int	fd;

	fd = open(redir->data, O_RDONLY);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(redir->data);
		close_used_pipes(pipefd);
		free_exit(cmd->info, cmds, 1);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("minishell: duplicate input to STDIN failure");
		close(fd);
		close_used_pipes(pipefd);
		free_exit(cmd->info, cmds, 1);
	}
	close(fd);
}

static void	child_output(t_cmd *cmd, t_redir *redir, t_vec *cmds, int pipefd[3])
{
	int	flags;
	int	fd;

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
		close_used_pipes(pipefd);
		free_exit(cmd->info, cmds, 1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("minishell: duplicate output to STDOUT failure");
		close(fd);
		close_used_pipes(pipefd);
		free_exit(cmd->info, cmds, 1);
	}
	close(fd);
}

void	child_redirections(t_cmd *cmd, int pipefd[3], t_vec *cmds)
{
	size_t	i;
	t_redir	*redir;

	i = 0;
	while (i < cmd->redirs.len)
	{
		redir = ft_vec_get(&cmd->redirs, i);
		if (redir->type == S_LT)
			child_input(cmd, redir, cmds, pipefd);
		else if (redir->type == D_LT)
		{
			if (redir->data && process_heredoc_str(&redir->data) != 0)
				free_exit(cmd->info, cmds, 1);
		}
		else if (redir->type == S_GT || redir->type == D_GT)
			child_output(cmd, redir, cmds, pipefd);
		i++;
	}
}

void	child_pipes(t_cmd *cmd, int pipefd[3], size_t i, t_vec *cmds)
{
	if (i > 0)
	{
		if (dup2(pipefd[PREV_READ], STDIN_FILENO) == -1)
		{
			perror("minishell: duplicate previous read to STDIN failure");
			free_exit(cmd->info, cmds, 1);
		}
	}
	if (i + 1 < cmds->len)
	{
		if (dup2(pipefd[WRITE_END], STDOUT_FILENO) == -1)
		{
			perror("minishell: duplicate write to STDOUT failure");
			free_exit(cmd->info, cmds, 1);
		}
	}
	close_used_pipes(pipefd);
}
