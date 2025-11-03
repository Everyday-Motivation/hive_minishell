/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 10:38:09 by timurray          #+#    #+#             */
/*   Updated: 2025/11/02 18:50:48 by timurray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute(t_vec *cmds, t_vec *env)
{
	size_t	i;
	t_cmd *cmd;
	
	pid_t	pid;
	int		status;
	
	int		in_fd;
	int		out_fd;
	int		pipefd[3];
	int		p_read;
	int 	p_write;

	pid_t last_pid;
	
	char **env_arr;

	pipefd[PREV_READ] = -1;
	p_read = -1;
	p_write = -1;

	if (!cmds)
		return (EXIT_FAILURE);
		
	i = 0;
	while (i < cmds->len)
	{
		cmd = (t_cmd *)ft_vec_get(cmds, i);


		if (i + 1 < cmds->len)
		{
			if(pipe(pipefd) == -1)
			{
				perror("pipe issue");
				if(pipefd[PREV_READ] != -1)
					close(pipefd[PREV_READ]);
				return (EXIT_FAILURE);
			}
			p_read = pipefd[READ_END];
			p_write = pipefd[WRITE_END];
		}
		else
		{
			p_read = -1;
			p_write = -1;
		}


		pid = fork();
		if (pid == -1)
		{
			perror("fork issue");
			if(i + 1 < cmds->len)
			{
				close(p_read);
				close(p_write);
			}
			if(pipefd[PREV_READ] != -1)
			{
				close(pipefd[PREV_READ]);
			}
			return (EXIT_FAILURE);
		}
		
		if (pid == CHILD)
		{

			if(cmd->input_file == NULL && cmd->heredoc_str == NULL && i > 0)
			{
				if(dup2(pipefd[PREV_READ], STDIN_FILENO) == -1)
				{
					perror("dup2 STDIN no in no here");
					if(pipefd[PREV_READ] != -1)
						close(pipefd[PREV_READ]);
					if(p_read != -1)
						close(p_read);
					if(p_write != -1)
						close(p_write);
					exit(1);
				}
			}

			if(cmd->output_file == NULL && (i + 1 <cmds->len ))
			{
				if(dup2(p_write, STDOUT_FILENO) == -1)
				{
					perror("dup2 STDOUT no out");
					if(pipefd[PREV_READ] != -1)
						close(pipefd[PREV_READ]);
					if(p_read != -1)
						close(p_read);
					if(p_write != -1)
						close(p_write);
					exit(1);
				}
			}

			if(pipefd[PREV_READ] != -1)
				close(pipefd[PREV_READ]);
			if(p_read != -1)
				close(p_read);
			if(p_write != -1)
				close(p_write);


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
					perror("dup2 in file issue");
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

			if(cmd->input_file == NULL && cmd->heredoc_str != NULL)
				process_heredoc_str(cmd);

			env_arr = vec_to_arr(env);

			char *cmd_path;
			cmd_path = search_path(cmd->argv[0], env);
			
			execve(cmd_path, cmd->argv, env_arr);
			perror("execution error");
			exit(1);
		}

		
		last_pid = pid;

		if (pipefd[PREV_READ] != -1)
		{
			close(pipefd[PREV_READ]);
			pipefd[PREV_READ] = -1;
		}
		if(i + 1 < cmds->len)
		{
			close(p_write);
			pipefd[PREV_READ] = p_read;
		}
		else
			p_read = -1;

		i++;
	}

	if (pipefd[PREV_READ] != -1)
		close(PREV_READ);
	

	if (waitpid(pid, &status, 0) == -1)
	{
		perror("wait pid child error in reaping");
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

// TODO: reaping the pids
// TODO: built ins in parent
// TODO; shrink functions

// TODO: pipe buffer max check?
// TODO: signal blocking?
