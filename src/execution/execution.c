/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 10:38:09 by timurray          #+#    #+#             */
/*   Updated: 2025/11/04 15:32:45 by timurray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute(t_vec *cmds, t_vec *env)
{
	size_t	i;
	t_cmd *cmd;
	
	pid_t	pid;
	int		status;
	pid_t last_pid;
	
	int		pipefd[3];
		
	char **env_arr;
	char *cmd_path;

	pipefd[READ_END] = -1;
	pipefd[WRITE_END] = -1;
	pipefd[PREV_READ] = -1;

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

			env_arr = vec_to_arr(env);


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
			close(pipefd[WRITE_END]);
			pipefd[PREV_READ] = pipefd[READ_END];
		}
		else
			pipefd[READ_END] = -1;

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
// TODO: free things? 
// TODO; shrink functions

// TODO: pipe buffer max check?
// TODO: signal blocking?
