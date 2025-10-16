/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaeklee <jaeklee@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 13:18:49 by jaeklee           #+#    #+#             */
/*   Updated: 2025/10/16 19:37:07 by jaeklee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	handle_heredoc(t_cmd *cmd, const char *limiter)
{
	char	*line;
	size_t	len;
	int		fd;

	if (cmd->heredoc_path == NULL)
	{
		// cmd->heredoc_path = FUNCTION_THAT_CREATES UNIQUE_FILENAME; e.g. .heredoc-0, .heredoc-1...
		cmd->heredoc_path = ".test";
	}
	fd = open(cmd->heredoc_path, O_CREAT | O_TRUNC | O_WRONLY, 0644);

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
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	close(fd);			 // write end
	cmd->input_fd = fd;	 // read end를 명령어의 input으로 설정
	return (1);
}

