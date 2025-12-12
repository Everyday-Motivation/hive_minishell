/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_child.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 14:24:26 by timurray          #+#    #+#             */
/*   Updated: 2025/12/12 11:27:13 by timurray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	child_redirect_input(t_cmd *cmd, int pipefd[3], t_vec *cmds)
{
	if (cmd->input_file)
	{
		pipefd[READ_END] = open(cmd->input_file, O_RDONLY);
		if (pipefd[READ_END] == -1)
		{
			ft_putstr_fd("minishell: ", 2);
			perror(cmd->input_file);
			free_exit(cmd->info, cmds, 1);
		}
		if (dup2(pipefd[READ_END], STDIN_FILENO) == -1)
		{
			perror("dup2 in file issue");
			close(pipefd[READ_END]);
			free_exit(cmd->info, cmds, 1);
		}
		close(pipefd[READ_END]);
	}
}

void	child_redirect_output(t_cmd *cmd, int pipefd[3], t_vec *cmds)
{
	if (cmd->output_file)
	{
		if (cmd->append)
			pipefd[WRITE_END] = open(cmd->output_file,
					O_WRONLY | O_CREAT | O_APPEND, 0644);
		else
			pipefd[WRITE_END] = open(cmd->output_file,
					O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (pipefd[WRITE_END] == -1)
		{
			ft_putstr_fd("minishell: ", 2);
			perror(cmd->output_file);
			free_exit(cmd->info, cmds, 1);
		}
		if (dup2(pipefd[WRITE_END], STDOUT_FILENO) == -1)
		{
			perror("dup2 out");
			close(pipefd[WRITE_END]);
			free_exit(cmd->info, cmds, 1);
		}
		close(pipefd[WRITE_END]);
	}
}

void	child_redirections(t_cmd *cmd, int pipefd[3], t_vec *cmds)
{
	child_redirect_input(cmd, pipefd, cmds);
	child_redirect_output(cmd, pipefd, cmds);
}

void	child_pipes(t_cmd *cmd, int pipefd[3], size_t i, t_vec *cmds)
{
	if (cmd->input_file == NULL && cmd->heredoc_str == NULL && i > 0)
	{
		if (dup2(pipefd[PREV_READ], STDIN_FILENO) == -1)
		{
			perror("dup2 STDIN no in no here");
			free_exit(cmd->info, cmds, 1);
		}
	}
	if (cmd->output_file == NULL && (i + 1 < cmds->len))
	{
		if (dup2(pipefd[WRITE_END], STDOUT_FILENO) == -1)
		{
			perror("dup2 STDOUT no out");
			free_exit(cmd->info, cmds, 1);
		}
	}
	close_used_pipes(pipefd);
}
