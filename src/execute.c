/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaeklee <jaeklee@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 12:30:31 by jaeklee           #+#    #+#             */
/*   Updated: 2025/09/29 17:34:57 by jaeklee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int execute_cmds(t_vec *cmds, t_vec *env)
{
    size_t i = 0;
    int pipe_fd[2];
    int prev_fd = -1; // Previous stdout (pipe read)
    pid_t pid;
    t_cmd *cmd;

	(void)env;
    while (i < cmds->len)
    {
        cmd = (t_cmd *)ft_vec_get(cmds, i);

        // check if it's not the last CMDs
        if (i < cmds->len - 1)
        {
            if (pipe(pipe_fd) == -1)
            {
                perror("pipe");
                return 0;
            }
        }
        pid = fork();
        if (pid == 0)
        {
            if (cmd->input_fd != 0)
                dup2(cmd->input_fd, STDIN_FILENO);
            else if (prev_fd != -1)
                dup2(prev_fd, STDIN_FILENO); // get input from pre-pipe

            if (cmd->output_fd != 1)
                dup2(cmd->output_fd, STDOUT_FILENO);
            else if (i < cmds->len - 1) //IF there is more pipe
                dup2(pipe_fd[1], STDOUT_FILENO); // send output to the next command

            if (prev_fd != -1) //if didn't modified
                close(prev_fd);
            if (i < cmds->len - 1) // if it's not the last
            {
                close(pipe_fd[0]);
                close(pipe_fd[1]);
            }
            execvp(cmd->argv[0], cmd->argv);
            perror("execvp"); // if fail
            _exit(1);
        }
        else if (pid < 0)
        {
            perror("fork");
            return 0;
        }
        if (prev_fd != -1)
            close(prev_fd);

        if (i < cmds->len - 1) // HERE you mod prev_fd for next pipe
        {
            close(pipe_fd[1]);
            prev_fd = pipe_fd[0]; 
        }

        i++;
    }
    while (wait(NULL) > 0); // wait chind P

    return (1);
}
