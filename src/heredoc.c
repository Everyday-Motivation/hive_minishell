/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaeklee <jaeklee@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 13:18:49 by jaeklee           #+#    #+#             */
/*   Updated: 2025/10/29 17:46:03 by jaeklee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	handle_heredoc(t_cmd *cmd, const char *limiter)
{
	char	*line;
	int		fd;
	char	*file_name;
	char	*gnl;
	char	*temp;

	file_name = "./heredoc_tmp";
	fd = open(file_name, O_CREAT | O_TRUNC | O_WRONLY, 0644);
	if (fd < 0)
	{
		perror("heredoc fd error");
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
		if (ft_strncmp(line, limiter, ft_strlen(limiter) + 1) == 0)
		{
			free(line);
			break;
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	close(fd);
	fd = open(file_name, O_RDONLY);
	if (fd < 0)
	{
		perror("heredoc fd error");
		return (0);
	}
	if (cmd->heredoc_str)
		free(cmd->heredoc_str);
	cmd->heredoc_str = ft_strdup("");
	if(!cmd->heredoc_str)
		return (0);
	while((gnl = get_next_line(fd)) != NULL)
	{
		temp = ft_strjoin(cmd->heredoc_str, gnl);
		free(cmd->heredoc_str);
		cmd->heredoc_str = temp;
		free(gnl);
		///temp EXCUTE에서 프리 아마도 룹돌리기
	}
	close(fd);
	unlink(file_name);
	return (1);
}

