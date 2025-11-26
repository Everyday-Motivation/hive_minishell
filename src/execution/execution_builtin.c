/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_builtin.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 14:49:28 by timurray          #+#    #+#             */
/*   Updated: 2025/11/26 17:21:51 by timurray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_bi(char *cmd)
{
	if (ft_strcmp(cmd, "cd") == 0)
		return (1);
	if (ft_strcmp(cmd, "echo") == 0)
		return (1);
	if (ft_strcmp(cmd, "env") == 0)
		return (1);
	if (ft_strcmp(cmd, "exit") == 0)
		return (1);
	if (ft_strcmp(cmd, "export") == 0)
		return (1);
	if (ft_strcmp(cmd, "pwd") == 0)
		return (1);
	if (ft_strcmp(cmd, "unset") == 0)
		return (1);
	return (0);
}

int	run_bi(char **argv, t_info *info, t_vec *cmds)
{
	if (ft_strcmp(argv[0], "cd") == 0)
		return (bi_cd(argv + 1, info));
	if (ft_strcmp(argv[0], "echo") == 0)
		return (bi_echo(argv + 1, info));
	if (ft_strcmp(argv[0], "env") == 0)
		return (bi_env(argv + 1, info));
	if (ft_strcmp(argv[0], "exit") == 0)
		return (bi_exit(argv + 1, info, cmds));
	if (ft_strcmp(argv[0], "export") == 0)
		return (bi_export(argv + 1, info));
	if (ft_strcmp(argv[0], "pwd") == 0)
		return (bi_pwd(argv + 1, info));
	if (ft_strcmp(argv[0], "unset") == 0)
		return (bi_unset(argv + 1, info));
	return (EXIT_FAILURE);
}

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
