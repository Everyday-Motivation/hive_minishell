/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaeklee <jaeklee@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 12:30:31 by jaeklee           #+#    #+#             */
/*   Updated: 2025/10/07 15:08:46 by jaeklee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*ft_strjoin_3(const char *s1, const char *s2, const char *s3)
{
	char	*res;
	size_t	len;

	len = ft_strlen(s1) + ft_strlen(s2) + ft_strlen(s3) + 1;
	res = malloc(len);
	if (!res)
		return (NULL);
	res[0] = '\0';
	ft_strlcpy(res, s1, len);
	printf("test = %s\n", res);
	ft_strlcat(res, s2, len);
	printf("test1 = %s\n", res);
	ft_strlcat(res, s3, len);
	printf("test2 = %s\n", res);
	return (res);
}

void	ft_free_split(char **arr)
{
	size_t	i;

	i = 0;
	if (!arr)
		return ;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

// char *search_path(char *cmd, t_vec *env)
// {
//     char **entry;
//     char *path_env = NULL;
//     char **paths;
//     char *full_path;
//     size_t i;
//     size_t j;

// 	j = 0;
//     while (j < env->len)
//     {
//         entry = (char **)ft_vec_get(env, j);
//         if (strncmp(*entry, "PATH=", 5) == 0)
//         {
//             path_env = *entry + 5;
//             break ;
//         }
//         j++;
//     }
//     if (!path_env)
//         return (NULL);
//     paths = ft_split(path_env, ':');
//     if (!paths)
//         return (NULL);
//     i = 0;
//     while (paths[i])
//     {
//         full_path = ft_strjoin_3(paths[i], "/", cmd);
//         if (!full_path)
//         {
//             ft_free_split(paths);
//             return (NULL);
//         }
//         if (access(full_path, X_OK) == 0)
//         {
//             ft_free_split(paths);
//             return (full_path);
//         }
//         free(full_path);
//         i++;
//     }
//     ft_free_split(paths);
//     return (NULL);
// }

char	*get_path_env(t_vec *env)
{
	size_t	j;
	char	**entry;

	j = 0;
	while (j < env->len)
	{
		entry = (char **)ft_vec_get(env, j);
		if (strncmp(*entry, "PATH=", 5) == 0)
			return (*entry + 5);
		j++;
	}
	return (NULL);
}

// char *find_executable_in_paths(char *path_env, char *cmd)
// {
//     char **paths;
//     char *full_path;
//     size_t i = 0;

// 	paths = ft_split(path_env, ':');
//     if (!paths)
//         return (NULL);
//     while (paths[i])
//     {
//         full_path = ft_strjoin_3(paths[i], "/", cmd);
//         if (!full_path)
//         {
//             ft_free_split(paths);
//             return (NULL);
//         }
//         if (access(full_path, X_OK) == 0)
//         {
//             ft_free_split(paths);
//             return (full_path);
//         }
//         free(full_path);
//         i++;
//     }
//     ft_free_split(paths);
//     return (NULL);
// }

char	*find_executable_in_paths(char *path_env, char *cmd)
{
	char	**paths;
	char	*full_path;
	size_t	i;

	paths = ft_split(path_env, ':');
	i = 0;
	if (!paths)
		return (NULL);
	while (paths[i])
	{
		full_path = ft_strjoin_3(paths[i++], "/", cmd);
		if (!full_path)
			break ;
		if (access(full_path, X_OK) == 0)
		{
			ft_free_split(paths);
			return (full_path);
		}
		free(full_path);
	}
	ft_free_split(paths);
	return (NULL);
}

char	*search_path(char *cmd, t_vec *env)
{
	char	*path_env;

	path_env = get_path_env(env);
	if (!path_env)
		return (NULL);
	return (find_executable_in_paths(path_env, cmd));
}

void	child_process(t_cmd *cmd, t_vec *env, t_exec_info *info)
{
	char	*exec_path;

	if (cmd->input_fd != STDIN_FILENO)
		dup2(cmd->input_fd, STDIN_FILENO);
	else if (info->prev_fd != -1)
	{
		dup2(info->prev_fd, STDIN_FILENO);
		close(info->prev_fd);
	}
	if (cmd->output_fd != STDOUT_FILENO)
		dup2(cmd->output_fd, STDOUT_FILENO);
	else if (info->index < info->total_cmds - 1)
	{
		dup2(info->pipe_fd[1], STDOUT_FILENO);
		close(info->pipe_fd[0]);
		close(info->pipe_fd[1]);
	}
	exec_path = search_path(cmd->argv[0], env);
	if (!exec_path)
	{
		fprintf(stderr, "%s: command not found\n", cmd->argv[0]);
		_exit(127);
	}
	execve(exec_path, cmd->argv, (char *const *)env->memory);
	perror("execve");
	_exit(1);
}

// void child_process(t_cmd *cmd, t_vec *env, t_exec_info *info)
// {
//     char *exec_path;

//     if (cmd->input_fd != STDIN_FILENO)
//         dup2(cmd->input_fd, STDIN_FILENO);
//     else if (info->prev_fd != -1)
//         dup2(info->prev_fd, STDIN_FILENO);
//     if (cmd->output_fd != STDOUT_FILENO)
//         dup2(cmd->output_fd, STDOUT_FILENO);
//     else if (info->index < info->total_cmds - 1)
//         dup2(info->pipe_fd[1], STDOUT_FILENO);
//     if (info->prev_fd != -1)
//         close(info->prev_fd);
//     if (info->index < info->total_cmds - 1)
//     {
//         close(info->pipe_fd[0]);
//         close(info->pipe_fd[1]);
//     }
//     exec_path = search_path(cmd->argv[0], env);
//     if (!exec_path)
//     {
//         fprintf(stderr, "%s: command not found\n", cmd->argv[0]);
//         _exit(127);
//     }
//     execve(exec_path, cmd->argv, (char * const *)env->memory);
//     perror("execve");
//     _exit(1);
// }
int	handle_post_fork(pid_t pid, t_exec_info *info, size_t i, size_t total_cmds)
{
	if (pid < 0)
	{
		perror("fork");
		return (0);
	}
	if (info->prev_fd != -1)
		close(info->prev_fd);
	if (i < total_cmds - 1)
	{
		close(info->pipe_fd[1]);
		info->prev_fd = info->pipe_fd[0];
	}
	return (1);
}

int	execute_cmds(t_vec *cmds, t_vec *env)
{
	size_t		i;
	pid_t		pid;
	t_cmd		*cmd;
	t_exec_info	info;

	i = 0;
	info.prev_fd = -1;
	info.total_cmds = cmds->len;
	while (i < cmds->len)
	{
		cmd = (t_cmd *)ft_vec_get(cmds, i);
		info.index = i;
		if (i < cmds->len - 1 && pipe(info.pipe_fd) == -1)
		{
			perror("pipe");
			return (0);
		}
		pid = fork();
		if (pid == 0)
			child_process(cmd, env, &info);
		else
		{
			if (!handle_post_fork(pid, &info, i, cmds->len))
				return (0);
		}
		i++;
	}
	while (wait(NULL) > 0)
		;
	return 1;
}

// int execute_cmds(t_vec *cmds, t_vec *env)
// {
//     size_t i;
//     pid_t pid;
//     t_cmd *cmd;
//     t_exec_info info;

//     i = 0;
//     info.prev_fd = -1;
//     info.total_cmds = cmds->len;
//     while (i < cmds->len)
//     {
//         cmd = (t_cmd *)ft_vec_get(cmds, i);
//         info.index = i;
//         if (i < cmds->len - 1 && pipe(info.pipe_fd) == -1) {
//             perror("pipe");
//             return 0;
//         }
//         pid = fork();
//         if (pid == 0) {
//             child_process(cmd, env, &info);
//         } else if (pid < 0) {
//             perror("fork");
//             return 0;
//         }
//         if (info.prev_fd != -1)
//             close(info.prev_fd);

//         if (i < cmds->len - 1) {
//             close(info.pipe_fd[1]);
//             info.prev_fd = info.pipe_fd[0];
//         }
//         i++;
//     }
//     while (wait(NULL) > 0);
//     return 1;
// }

// int execute_cmds(t_vec *cmds, t_vec *env)
// {
//     size_t i;
//     int pipe_fd[2];
//     int prev_fd;
//     pid_t pid;
//     t_cmd *cmd;
//     char *exec_path;

// 	i = 0;
// 	prev_fd = -1;
//     while (i < cmds->len)
//     {
//         cmd = (t_cmd *)ft_vec_get(cmds, i);

//         if (i < cmds->len - 1 && pipe(pipe_fd) == -1)
//         {
//             perror("pipe");
//             return 0;
//         }
//         pid = fork();
//         if (pid == 0)
//         {
//             if (cmd->input_fd != 0)
//                 dup2(cmd->input_fd, STDIN_FILENO);
//             else if (prev_fd != -1)
//                 dup2(prev_fd, STDIN_FILENO);
//             if (cmd->output_fd != 1)
//                 dup2(cmd->output_fd, STDOUT_FILENO);
//             else if (i < cmds->len - 1)
//                 dup2(pipe_fd[1], STDOUT_FILENO);
//             if (prev_fd != -1)
//                 close(prev_fd);
//             if (i < cmds->len - 1)
//             {
//                 close(pipe_fd[0]);
//                 close(pipe_fd[1]);
//             }

//             exec_path = search_path(cmd->argv[0], env);
//             if (!exec_path)
//             {
//                 fprintf(stderr, "%s: command not found\n", cmd->argv[0]);
//                 _exit(127);
//             }
//             execve(exec_path, cmd->argv, (char * const *)env->memory);
//             perror("execve");
//             _exit(1);
//         }
//         else if (pid < 0)
//         {
//             perror("fork");
//             return 0;
//         }
//         if (prev_fd != -1)
//             close(prev_fd);

//         if (i < cmds->len - 1)
//         {
//             close(pipe_fd[1]);
//             prev_fd = pipe_fd[0];
//         }

//         i++;
//     }

//     while (wait(NULL) > 0);
//     return (1);
// }
