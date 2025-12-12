/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 10:38:09 by timurray          #+#    #+#             */
/*   Updated: 2025/12/12 08:55:19 by timurray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	child_error(char *path, char **argv, char **env)
{
	struct stat	sb;
	int			error_code;
	char		*error_msg;
	char		*tmp;

	error_code = errno;
	if (path && stat(path, &sb) == 0 && S_ISDIR(sb.st_mode))
	{
		error_msg = ft_strjoin_3("minishell: ", argv[0], ": Is a directory\n");
		if (error_msg)
		{
			write(2, error_msg, ft_strlen(error_msg));
			free(error_msg);
		}
		error_code = 126;
	}
	else
	{
		tmp = ft_strjoin_3("minishell: ", argv[0], ": ");
		if (tmp)
		{
			error_msg = ft_strjoin_3(tmp, strerror(errno), "\n");
			free(tmp);
			if (error_msg)
			{
				write(2, error_msg, ft_strlen(error_msg));
				free(error_msg);
			}
		}
		if (error_code == ENOENT)
			error_code = 127;
		else
			error_code = 126;
	}
	free(env);
	if (path && !ft_strchr(argv[0], '/'))
		free(path);
	return (error_code);
}

void	child_run(t_cmd *cmd, t_info *info, t_vec *cmds)
{
	char	**env;
	char	*path;
	char	*underscore;

	if (ft_strchr(cmd->argv[0], '/'))
		path = cmd->argv[0];
	else
		path = search_path(cmd->argv[0], info->env);
	if (path)
	{
		underscore = ft_strjoin("_=", path);
		if (underscore)
		{
			env_add_update_line(info->env, underscore);
			free(underscore);
		}
	}
	env = vec_to_arr(info->env);
	if (!env)
		free_exit(info, cmds, 1);
	if (!ft_strchr(cmd->argv[0], '/') && !path)
		no_path_exit(cmd, env, info, cmds);
	execve(path, cmd->argv, env);
	free_exit(info, cmds, child_error(path, cmd->argv, env));
}

void	child_process(t_vec *cmds, t_info *info, int pipefd[3], size_t i)
{
	t_cmd	*cmd;

	cmd = (t_cmd *)ft_vec_get(cmds, i);
	child_sig();
	child_pipes(cmd, pipefd, i, cmds);
	child_redirections(cmd, pipefd, cmds);
	if (cmd->input_file == NULL && cmd->heredoc_str != NULL)
	{
		if (process_heredoc_str(cmd) != 0)
			free_exit(info, cmds, 1);
	}
	if (cmd->argv == NULL || cmd->argv[0] == NULL)
		free_exit(info, cmds, 0);
	if (cmd->argv[0][0] == '\0')
	{
		ft_putstr_fd("minishell: : command not found\n", 2);
		free_exit(info, cmds, 127);
	}
	if (is_bi(cmd->argv[0]) == 1)
		free_exit(info, cmds, run_bi(cmd->argv, info, cmds));
	child_run(cmd, info, cmds);
}

int	execute(t_vec *cmds, t_info *info)
{
	int		pipefd[3];
	int		status;
	pid_t	pid;
	size_t	i;

	init_pipes(pipefd);
	status = parent_builtin(cmds, info);
	if (status != -1)
		return (status);
	i = 0;
	while (i < cmds->len)
	{
		if (signal_pipe(pipefd, i, cmds) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		pid = fork();
		if (pid == -1)
			return (fork_error(pipefd));
		if (pid == CHILD)
			child_process(cmds, info, pipefd, i);
		recycle_pipes(pipefd, i++, cmds);
	}
	if (pipefd[PREV_READ] != -1)
		close(pipefd[PREV_READ]);
	return (reap_zombies(pid, cmds->len));
}
