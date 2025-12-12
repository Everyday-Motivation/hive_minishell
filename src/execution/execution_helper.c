/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_helper.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 12:58:29 by timurray          #+#    #+#             */
/*   Updated: 2025/12/12 11:45:27 by timurray         ###   ########.fr       */
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
	char	*underscore;

	underscore = ft_strjoin("_=", argv[0]);
	if (underscore)
	{
		env_add_update_line(info->env, underscore);
		free(underscore);
	}
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

int	fork_error(int pipefd[3])
{
	perror("fork issue");
	close_used_pipes(pipefd);
	return (EXIT_FAILURE);
}

void	no_path_exit(t_cmd *cmd, char **env, t_info *info, t_vec *cmds)
{
	char	*error_msg;

	error_msg = ft_strjoin(cmd->argv[0], ": command not found\n");
	if (error_msg)
	{
		write(2, error_msg, ft_strlen(error_msg));
		free(error_msg);
	}
	free(env);
	free_exit(info, cmds, 127);
}

void	child_error_free(char *path, char **argv, char **env)
{
	free(env);
	if (path && !ft_strchr(argv[0], '/'))
		free(path);
}
