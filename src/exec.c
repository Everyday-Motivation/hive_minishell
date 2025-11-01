/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 10:38:09 by timurray          #+#    #+#             */
/*   Updated: 2025/11/01 19:02:04 by timurray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// loop through commands and run builtins first

int	execute(t_vec *cmds, t_vec *env)
{
	t_cmd *cmd;
	size_t	i;
	
	pid_t	pid;
	int		status;
	
	// int pipefd[3];
	int		in_fd;
	int		out_fd;
	
	char **env_arr;

	if (!cmds)
		return (EXIT_FAILURE);
	i = 0;
	while (i < cmds->len)
	{
		cmd = (t_cmd *)ft_vec_get(cmds, i);

		pid = fork();
		if (pid == -1)
		{
			perror("fork issue");
			return (EXIT_FAILURE);
		}
		
		if (pid == CHILD)
		{
			if (cmd->input_file)
			{
				in_fd = open(cmd->input_file, O_RDONLY);
				if (in_fd == -1)
				{
					perror("open in file issue");
					exit(1);
				}
				if (dup2(in_fd, STDIN_FILENO) == -1)
				{
					perror("dup2 in");
					close(in_fd);
					exit(1);
				}
				close(in_fd);
			}
			if (cmd->output_file)
			{
				if (cmd->append)
					out_fd = open(cmd->output_file,
							O_WRONLY | O_CREAT | O_APPEND, 0644);
				else
					out_fd = open(cmd->output_file,
							O_WRONLY | O_CREAT | O_TRUNC, 0644);
				if (out_fd == -1)
				{
					perror("open outfile issue");
					exit(1);
				}
				if (dup2(out_fd, STDOUT_FILENO) == -1)
				{
					perror("dup2 out");
					close(out_fd);
					exit(1);
				}
				close(out_fd);
			}

			env_arr = vec_to_arr(env);

			printf("cmd->argv[0]:%s\n", cmd->argv[0]);
			char *cmd_path;
			cmd_path = search_path(cmd->argv[0], env);
			
			execve(cmd_path, cmd->argv, env_arr);
			perror("execution error");
			exit(1);
		}
		i++;
	}


	if (waitpid(pid, &status, 0) == -1)
	{
		perror("wait pid child error in reaping");
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

// TODO: built ins in parent
// TODO: main function to accept the vector
// TODO: resuse pipes

// TODO: pipe buffer max check?
// TODO: signal blocking?
