/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaeklee <jaeklee@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 13:18:49 by jaeklee           #+#    #+#             */
/*   Updated: 2025/11/05 16:30:13 by jaeklee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	open_heredoc_file(char *file_name)
{
	int	fd;

	fd = open(file_name, O_CREAT | O_TRUNC | O_WRONLY, 0644);
	if (fd < 0)
		perror("heredoc fd error");
	return (fd);
}

static void	write_to_fd(char *expanded, int fd, char *line)
{
	write(fd, expanded, ft_strlen(expanded));
	write(fd, "\n", 1);
	free(line);
}

static int	write_heredoc_input(t_cmd *cmd, int fd, char *limiter,
		int quote_flag)
{
	char	*line;
	char	*expanded;

	expanded = NULL;
	while (g_signal != SIGINT)
	{
		line = readline("> ");
		if (!line)
		{
			write(STDERR_FILENO, "warning: heredoc delimited by end-of-file\n",
				43);
			break ;
		}
		if (ft_strncmp(line, limiter, ft_strlen(limiter) + 1) == 0)
		{
			free(line);
			break ;
		}
		if (quote_flag == 0)
			expanded = expand_env_in_heredoc_line(cmd->info, line);
		if (!expanded)
			expanded = ft_strdup(line);
		write_to_fd(expanded, fd, line);
	}
	return (0);
}

static char	*read_heredoc_content(char *file_name)
{
	int		fd;
	char	*result;
	char	*gnl;
	char	*temp;

	fd = open_heredoc_file_rdonly(file_name);
	if (fd < 0)
		return (NULL);
	result = ft_strdup("");
	if (!result)
	{
		close(fd);
		return (NULL);
	}
	gnl = get_next_line(fd);
	while (gnl != NULL)
	{
		temp = ft_strjoin(result, gnl);
		free(result);
		free(gnl);
		result = temp;
		gnl = get_next_line(fd);
	}
	close(fd);
	return (result);
}
// AFRER HEREDOC = free(temp), free(result);

int	handle_heredoc(t_cmd *cmd, t_token *limiter)
{
	char	*file_name;
	int		fd;
	int		quote_flag;

	quote_flag = limiter_check(limiter->raw_data);
	file_name = "./heredoc_tmp";
	fd = open_heredoc_file(file_name);
	if (fd < 0)
		return (0);
	if (g_signal == SIGINT)
	{
		cmd->info->exit_code = 130;
		close_unlink_heredoc(fd, file_name);
		write(STDOUT_FILENO, "\n", 1);
		return (0);
	}
	write_heredoc_input(cmd, fd, limiter->data, quote_flag);
	close(fd);
	if (cmd->heredoc_str)
		free(cmd->heredoc_str);
	cmd->heredoc_str = read_heredoc_content(file_name);
	unlink(file_name);
	if (!cmd->heredoc_str)
		return (0);
	return (1);
}
