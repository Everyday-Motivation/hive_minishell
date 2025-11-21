/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 10:38:09 by timurray          #+#    #+#             */
/*   Updated: 2025/11/21 15:58:04 by timurray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void init_pipes(int pipefd[3])
{
	pipefd[READ_END] = -1;
	pipefd[WRITE_END] = -1;
	pipefd[PREV_READ] = -1;
}

int	execute(t_vec *cmds, t_vec *env)
{
	t_cmd	*cmd;
	int		pipefd[3];
	size_t	i;
	size_t	child_count;
	size_t	reaped;
	
	pid_t	pid;
	pid_t	last_pid;
	pid_t	waited_pid;
	int		status;
	int		last_status;

	int bi_status;
	int error_code;

	char **env_arr;
	char *cmd_path;

	init_pipes(pipefd);		
	i = 0;
	child_count = 0;
	last_pid = -1;
	while (i < cmds->len)
	{
		cmd = (t_cmd *)ft_vec_get(cmds, i);

		if (is_bi(cmd->argv[0]) == 1 && cmds->len == 1)
			return (run_bi(cmd->argv, env));

		if (i + 1 < cmds->len)
		{
			if(pipe(pipefd) == -1)
			{
				perror("pipe issue");
				if(pipefd[PREV_READ] != -1)
					close(pipefd[PREV_READ]);
				return (EXIT_FAILURE);
			}
		}
		else
		{
			pipefd[READ_END] = -1;
			pipefd[WRITE_END] = -1;
		}


		pid = fork();
		if (pid == -1)
		{
			perror("fork issue");
			if(i + 1 < cmds->len)
			{
				close(pipefd[READ_END]);
				close(pipefd[WRITE_END]);
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
					if(pipefd[READ_END] != -1)
						close(pipefd[READ_END]);
					if(pipefd[WRITE_END] != -1)
						close(pipefd[WRITE_END]);
					exit(1);
				}
			}

			if(cmd->output_file == NULL && (i + 1 <cmds->len ))
			{
				if(dup2(pipefd[WRITE_END], STDOUT_FILENO) == -1)
				{
					perror("dup2 STDOUT no out");
					if(pipefd[PREV_READ] != -1)
						close(pipefd[PREV_READ]);
					if(pipefd[READ_END] != -1)
						close(pipefd[READ_END]);
					if(pipefd[WRITE_END] != -1)
						close(pipefd[WRITE_END]);
					exit(1);
				}
			}

			if(pipefd[PREV_READ] != -1)
				close(pipefd[PREV_READ]);
			if(pipefd[READ_END] != -1)
				close(pipefd[READ_END]);
			if(pipefd[WRITE_END] != -1)
				close(pipefd[WRITE_END]);


			if (cmd->input_file)
			{
				pipefd[READ_END] = open(cmd->input_file, O_RDONLY);
				if (pipefd[READ_END] == -1)
				{
					perror("open in file issue");
					exit(1);
				}
				if (dup2(pipefd[READ_END], STDIN_FILENO) == -1)
				{
					perror("dup2 in file issue");
					close(pipefd[READ_END]);
					exit(1);
				}
				close(pipefd[READ_END]);
			}

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
					perror("open outfile issue");
					exit(1);
				}
				if (dup2(pipefd[WRITE_END], STDOUT_FILENO) == -1)
				{
					perror("dup2 out");
					close(pipefd[WRITE_END]);
					exit(1);
				}
				close(pipefd[WRITE_END]);
			}

			if(cmd->input_file == NULL && cmd->heredoc_str != NULL)
				process_heredoc_str(cmd);

			
			if (is_bi(cmd->argv[0]) == 1)
			{
				bi_status = run_bi(cmd->argv, env);
				exit(bi_status);
			}
			else
			{
				env_arr = vec_to_arr(env);
				cmd_path = search_path(cmd->argv[0], env);
				if(!cmd_path)
				{
					ft_putstr_fd("minishell: command not found: ", 2);
					ft_putendl_fd(cmd->argv[0], 2);
					free(env_arr);
					exit(127);
				}
				
				execve(cmd_path, cmd->argv, env_arr);
				
				error_code = errno;
				perror("execution error");
				free(env_arr);
				free(cmd_path);
				if (error_code == ENOENT)
					exit(127);
				else
					exit(126);
			}
		}
		child_count++;
		
		last_pid = pid;

		if (pipefd[PREV_READ] != -1)
		{
			close(pipefd[PREV_READ]);
			pipefd[PREV_READ] = -1;
		}
		if(i + 1 < cmds->len)
		{
			close(pipefd[WRITE_END]);
			pipefd[PREV_READ] = pipefd[READ_END];
		}
		else
			pipefd[READ_END] = -1;

		i++;
	}

	if (pipefd[PREV_READ] != -1)
		close(pipefd[PREV_READ]);
	
	status = 0;
	last_status = 0;
	reaped = 0;
	while (reaped < child_count)
	{
		waited_pid = wait(&status);
		if(waited_pid == -1)
		{
			perror("waited pid issue");
			return (EXIT_FAILURE);
		}
		if (waited_pid == last_pid)
			last_status = status;
		reaped++;
	}
	if (WIFEXITED(last_status))
		return (WEXITSTATUS(last_status));

	return (EXIT_FAILURE);
}

// TODO: free things? In exit.
// TODO: update  _?

// TODO: pipe buffer max check?
// TODO: signal blocking?
// TODO; shrink functions