/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 13:18:49 by jaeklee           #+#    #+#             */
/*   Updated: 2025/12/13 11:01:48 by timurray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	create_heredoc_file(char *file_name)
{
	int	fd_file_name;
	int	fd;

	fd_file_name = open("/proc/sys/kernel/random/uuid", O_RDONLY);
	if (fd_file_name < 0)
	{
		ft_putendl_fd("open /proc/sys/kernel/random/uuid failed", 2);
		return (-1);
	}
	file_name[36] = '\0';
	if (read(fd_file_name, file_name, 36) <= 0)
	{
		ft_putendl_fd("read uuid failed", 2);
		close(fd_file_name);
		return (-1);
	}
	close(fd_file_name);
	fd = open(file_name, O_CREAT | O_TRUNC | O_WRONLY, 0644);
	if (fd < 0)
	{
		ft_putendl_fd("heredoc fd error", 2);
		return (-1);
	}
	return (fd);
}

static void	write_to_fd(char *expanded, int fd, char *line)
{
	write(fd, expanded, ft_strlen(expanded));
	write(fd, "\n", 1);
	free(line);
}

// static int	write_heredoc_input(t_cmd *cmd, int fd, char *limiter,
// 		int quote_flag)
static int	write_heredoc_input(t_info *info, int fd, char *limiter,
		int quote_flag)
{
	char	*line;
	char	*expanded;

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
		expanded = NULL;
		if (quote_flag == 0)
			expanded = expand_env_in_heredoc_line(info, line);
			// expanded = expand_env_in_heredoc_line(cmd->info, line);
		if (!expanded)
			expanded = arena_strdup(info->arena, line, ft_strlen(line));
			// expanded = arena_strdup(cmd->info->arena, line, ft_strlen(line));
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
// int	handle_heredoc(t_cmd *cmd, t_token *limiter)
int	handle_heredoc(t_info *info, t_token *limiter, char **content)
{
	char	file_name[37];
	int		fd;
	int		quote_flag;
	int		ret;

	quote_flag = limiter_check(limiter->raw_data);
	if (quote_flag == 1)
		del_quotes(limiter->raw_data);
	fd = create_heredoc_file(file_name);
	if (fd < 0)
		return (0);
	// write_heredoc_input(cmd, fd, limiter->raw_data, quote_flag);
	write_heredoc_input(info, fd, limiter->raw_data, quote_flag);
	// ret = handle_heredoc_signal(cmd, fd, file_name);
	ret = handle_heredoc_signal(info, fd, file_name);
	if (ret != 0)
		return (ret);
	close(fd);
	// if (cmd->heredoc_str)
	// 	free(cmd->heredoc_str);
	// cmd->heredoc_str = read_heredoc_content(file_name);
	*content = read_heredoc_content(file_name);
	unlink(file_name);
	// if (!cmd->heredoc_str)
	// 	return (0);
	if (!*content)
		return (0);
	return (1);
}
