
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 10:38:09 by timurray          #+#    #+#             */
/*   Updated: 2025/11/26 13:56:14 by timurray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void child_process(t_vec *cmds, t_info *info, int pipefd[3],size_t i)
{
	t_cmd	*cmd;
	char **env_arr;
	char *cmd_path;
	int error_code;

	cmd = (t_cmd *)ft_vec_get(cmds, i);
	child_sig();
	child_pipes(cmd, pipefd, i, cmds);
	child_redirections(cmd, pipefd);
	if(cmd->input_file == NULL && cmd->heredoc_str != NULL)
	{
		if (process_heredoc_str(cmd) != 0)
			exit(1);
	}
	if (cmd->argv == NULL || cmd->argv[0] == NULL || cmd->argv[0][0] == '\0')
		exit(0);
	if (is_bi(cmd->argv[0]) == 1)
		exit(run_bi(cmd->argv, info, cmds));
	else
	{
		env_arr = vec_to_arr(info->env);
		if(!env_arr)
		{
			perror("env arr issue");
			exit(1);
		}
		cmd_path = NULL;
		if(ft_strchr(cmd->argv[0], '/'))
			cmd_path = cmd->argv[0];
		else
		{				
			cmd_path = search_path(cmd->argv[0], info->env);
			if(!cmd_path)
			{
				ft_putstr_fd(cmd->argv[0], 2);
				ft_putendl_fd(": command not found", 2);
				free(env_arr);
				exit(127);
			}
		}
		execve(cmd_path, cmd->argv, env_arr);
		
		error_code = errno;
		struct stat sb;
		if (stat(cmd_path, &sb) == 0 && S_ISDIR(sb.st_mode))
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(cmd->argv[0], 2);
			ft_putendl_fd(": Is a directory", 2);
			free(env_arr);
			if(!ft_strchr(cmd->argv[0], '/') && cmd_path != NULL)
				free(cmd_path);
			exit(126);
		}
		ft_putstr_fd("minishell: ", 2);
		perror(cmd->argv[0]);
		free(env_arr);
		if(!ft_strchr(cmd->argv[0], '/') && cmd_path != NULL)
			free(cmd_path);
		if (error_code == ENOENT)
			exit(127);
		else
			exit(126);
	}
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

/* TEST
TODO: echo hello | cat | cat | cat | exit

dir1/dir2/dir3/ then "rm -r dir2"
cat + ctrl c should exit
cat + ctrl \ should print Quite core dumped

TODO: update  _?
TODO: pipe buffer max check?
TODO: signal blocking?
TODO; shrink functions
 */
