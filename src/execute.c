/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaeklee <jaeklee@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 12:30:31 by jaeklee           #+#    #+#             */
/*   Updated: 2025/10/02 15:52:30 by jaeklee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"


char *ft_strjoin_3(const char *s1, const char *s2, const char *s3)
{
    char *res;
    size_t len;

    len = ft_strlen(s1) + ft_strlen(s2) + ft_strlen(s3) + 1;

    res = malloc(len);
    if (!res)
        return NULL;

    res[0] = '\0';
    ft_strlcpy(res, s1, len);
	printf("test = %s\n", res);
    ft_strlcat(res, s2, len);
	printf("test1 = %s\n", res);
    ft_strlcat(res, s3, len);
	printf("test2 = %s\n", res);

    return (res);
}

void ft_free_split(char **arr)
{
    size_t i = 0;

    if (!arr)
        return;

    while (arr[i])
    {
        free(arr[i]);
        i++;
    }
    free(arr);
}

char *search_path(char *cmd, t_vec *env)
{
    char **entry;
    char *path_env = NULL;
    char **paths;
    char *full_path;
    size_t i;
    size_t j;
	
	j = 0;
    while (j < env->len)
    {
        entry = (char **)ft_vec_get(env, j);
        if (strncmp(*entry, "PATH=", 5) == 0)
        {
            path_env = *entry + 5;
            break;
        }
        j++;
    }

    if (!path_env)
        return (NULL);
    paths = ft_split(path_env, ':');
    if (!paths)
        return (NULL);

    i = 0;
    while (paths[i])
    {
        full_path = ft_strjoin_3(paths[i], "/", cmd);
        if (!full_path)
        {
            ft_free_split(paths);
            return (NULL);
        }
        if (access(full_path, X_OK) == 0)
        {
            ft_free_split(paths);
            return (full_path);
        }

        free(full_path);
        i++;
    }

    ft_free_split(paths);
    return (NULL);
}

int execute_cmds(t_vec *cmds, t_vec *env)
{
    size_t i;
    int pipe_fd[2];
    int prev_fd;
    pid_t pid;
    t_cmd *cmd;
    char *exec_path;

	i = 0;
	prev_fd = -1;
    while (i < cmds->len)
    {
        cmd = (t_cmd *)ft_vec_get(cmds, i);

        if (i < cmds->len - 1 && pipe(pipe_fd) == -1)
        {
            perror("pipe");
            return 0;
        }
        pid = fork();
        if (pid == 0)
        {
            if (cmd->input_fd != 0)
                dup2(cmd->input_fd, STDIN_FILENO);
            else if (prev_fd != -1)
                dup2(prev_fd, STDIN_FILENO);
            if (cmd->output_fd != 1)
                dup2(cmd->output_fd, STDOUT_FILENO);
            else if (i < cmds->len - 1)
                dup2(pipe_fd[1], STDOUT_FILENO);
            if (prev_fd != -1)
                close(prev_fd);
            if (i < cmds->len - 1)
            {
                close(pipe_fd[0]);
                close(pipe_fd[1]);
            }

            exec_path = search_path(cmd->argv[0], env);
            if (!exec_path)
            {
                fprintf(stderr, "%s: command not found\n", cmd->argv[0]);
                _exit(127);
            }
            execve(exec_path, cmd->argv, (char * const *)env->memory);
            perror("execve");
            _exit(1);
        }
        else if (pid < 0)
        {
            perror("fork");
            return 0;
        }
        if (prev_fd != -1)
            close(prev_fd);

        if (i < cmds->len - 1)
        {
            close(pipe_fd[1]);
            prev_fd = pipe_fd[0];
        }

        i++;
    }

    while (wait(NULL) > 0);
    return (1);
}

// int execute_cmds(t_vec *cmds, t_vec *env)
// {
//     size_t i = 0;
//     int pipe_fd[2];
//     int prev_fd = -1; // Previous stdout (pipe read)
//     pid_t pid;
//     t_cmd *cmd;

// 	(void)env;
//     while (i < cmds->len)
//     {
//         cmd = (t_cmd *)ft_vec_get(cmds, i);

//         // check if it's not the last CMDs
//         if (i < cmds->len - 1)
//         {
//             if (pipe(pipe_fd) == -1)
//             {
//                 perror("pipe");
//                 return 0;
//             }
//         }
//         pid = fork();
//         if (pid == 0)
//         {
//             if (cmd->input_fd != 0)
//                 dup2(cmd->input_fd, STDIN_FILENO);
//             else if (prev_fd != -1)
//                 dup2(prev_fd, STDIN_FILENO); // get input from pre-pipe

//             if (cmd->output_fd != 1)
//                 dup2(cmd->output_fd, STDOUT_FILENO);
//             else if (i < cmds->len - 1) //IF there is more pipe
//                 dup2(pipe_fd[1], STDOUT_FILENO); // send output to the next command

//             if (prev_fd != -1) //if didn't modified
//                 close(prev_fd);
//             if (i < cmds->len - 1) // if it's not the last
//             {
//                 close(pipe_fd[0]);
//                 close(pipe_fd[1]);
//             }
// 			execvp(cmd->argv[0], cmd->argv);
// 			if (execve(cmd->argv[0], cmd->argv, (char * const *)env->memory) == -1)
// 			{
// 				perror("execve");
// 				_exit(1);
// 			}
//             perror("execvp"); // if fail
//             _exit(1);
//         }
//         else if (pid < 0)
//         {
//             perror("fork");
//             return 0;
//         }
//         if (prev_fd != -1)
//             close(prev_fd);

//         if (i < cmds->len - 1) // HERE you mod prev_fd for next pipe
//         {
//             close(pipe_fd[1]);
//             prev_fd = pipe_fd[0]; 
//         }

//         i++;
//     }
//     while (wait(NULL) > 0); // wait chind P

//     return (1);
// }
