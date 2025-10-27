/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaeklee <jaeklee@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 13:18:49 by jaeklee           #+#    #+#             */
/*   Updated: 2025/10/27 12:13:41 by jaeklee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// int	handle_heredoc(t_cmd *cmd, const char *limiter)
// {
// 	char	*line;
// 	size_t	len;
// 	int		fd;
// 	char	*file_name;

// 	// temp_name = ft_strjoin(".heredoc_",ft_itoa(cmd->heredoc_counter) );
// 	if ()
// 	file_name = "./heredoc_tmp";
		
	
// 	// if (cmd->heredoc_path == NULL)
// 	// {
// 	// 	// cmd->heredoc_path = FUNCTION_THAT_CREATES UNIQUE_FILENAME; e.g. .heredoc-0, .heredoc-1...
// 	// 	cmd->heredoc_path = temp_name;
// 	// }
// 	fd = open(cmd->heredoc_path, O_CREAT | O_TRUNC | O_WRONLY, 0644);

// 	while (1)
// 	{
// 		line = readline("> ");
// 		if (!line)
// 		{
// 			write(STDERR_FILENO, "warning: heredoc delimited by end-of-file\n", 43);
// 			break;
// 		}
// 		// '\n' -> '\0'
//  		len = ft_strlen(line);
// 		if (len > 0 && line[len - 1] == '\n')
// 			line[len - 1] = '\0';
// 		// if line == limiter
// 		if (ft_strncmp(line, limiter, ft_strlen(limiter) + 1) == 0)
// 		{
// 			free(line);
// 			break;
// 		}
// 		// 입력 내용을 pipe에 쓰기
// 		write(fd, line, ft_strlen(line));
// 		write(fd, "\n", 1);
// 		free(line);
// 	}
// 	close(fd);			 // write end
// 	cmd->input_fd = open(cmd->heredoc_path, O_RDONLY);
// 	return (1);
// }

int	handle_heredoc(t_cmd *cmd, const char *limiter)
{
	char	*line;
	size_t	len;
	int		fd;
	char	*file_name;

	// temp_name = ft_strjoin(".heredoc_",ft_itoa(cmd->heredoc_counter) );
	if (cmd -> heredoc_fd > STDERR_FILENO)
	{
		
	}
	file_name = "./heredoc_tmp";
	cmd->heredoc_fd = open(file_name, O_CREAT | O_TRUNC | O_WRONLY, 0644);

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
	cmd->input_fd = open(cmd->heredoc_path, O_RDONLY);
	return (1);
}

