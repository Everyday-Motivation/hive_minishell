/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaeklee <jaeklee@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 13:18:49 by jaeklee           #+#    #+#             */
/*   Updated: 2025/10/01 15:53:00 by jaeklee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	handle_heredoc(t_cmd *cmd, const char *limiter)
{
	int		pipefd[2];
	char	*line;
	size_t	len;

	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		return (0);
	}

	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			write(STDERR_FILENO, "warning: heredoc delimited by end-of-file\n", 43);
			break;
		}
		// '\n' -> '\0'
 		len = ft_strlen(line);
		if (len > 0 && line[len - 1] == '\n')
			line[len - 1] = '\0';
		// if line == limiter
		if (ft_strncmp(line, limiter, ft_strlen(limiter) + 1) == 0)
		{
			free(line);
			break;
		}
		// 입력 내용을 pipe에 쓰기
		write(pipefd[1], line, ft_strlen(line));
		write(pipefd[1], "\n", 1);
		free(line);
	}
	close(pipefd[1]);			 // write end
	cmd->input_fd = pipefd[0];	 // read end를 명령어의 input으로 설정
	return (1);
}

