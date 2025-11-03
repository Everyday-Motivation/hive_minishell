/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaeklee <jaeklee@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 13:18:49 by jaeklee           #+#    #+#             */
/*   Updated: 2025/11/03 13:49:34 by jaeklee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern volatile sig_atomic_t	g_signal;

char	*expand_env_in_heredoc_line(t_info *info, char *input)
{
	size_t	i;
	size_t	buf_i;
	char	*buf;

	buf = arena_alloc(info->arena, ft_strlen(input));
	i = 0;
	buf_i = 0;
	while (input[i])
	{
		if (input[i] == '"')
			buf_i = handle_double_quote(info, input, &i, &buf);
		else if (input[i] == '$')
			buf_i = handle_env_variable(info, input, &i, &buf);
		else
			buf[buf_i++] = input[i++];
	}
	buf[buf_i] = '\0';
	return (buf);
}

static int	open_heredoc_file(const char *file_name)
{
	int	fd;

	fd = open(file_name, O_CREAT | O_TRUNC | O_WRONLY, 0644);
	if (fd < 0)
		perror("heredoc fd error");
	return (fd);
}

static int	write_heredoc_input(t_cmd *cmd, int fd, const char *limiter)
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
		expanded = expand_env_in_heredoc_line(cmd->info, line);
		if (!expanded)
			expanded = ft_strdup(line);
		write(fd, expanded, ft_strlen(expanded));
		write(fd, "\n", 1);
		free(line);
	}
	return (0);
}

static char	*read_heredoc_content(const char *file_name)
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

int	handle_heredoc(t_cmd *cmd, const char *limiter)
{
	char	*file_name;
	int		fd;

	file_name = "./heredoc_tmp";
	fd = open_heredoc_file(file_name);
	if (fd < 0)
		return (0);
	if (g_signal == SIGINT)
	{
		close(fd);
		unlink(file_name);
		write(STDOUT_FILENO, "\n", 1);
		return (0);
	}
	write_heredoc_input(cmd, fd, limiter);
	close(fd);
	if (cmd->heredoc_str)
		free(cmd->heredoc_str);
	cmd->heredoc_str = read_heredoc_content(file_name);
	unlink(file_name);
	if (!cmd->heredoc_str)
		return (0);
	printf("HEREDOC CONTENT:%s", cmd->heredoc_str);
	return (1);
}

// int	handle_heredoc(t_cmd *cmd, const char *limiter)
// {
// 	char	*line;
// 	int		fd;
// 	char	*file_name;
// 	char	*gnl;
// 	char	*temp;
//     char    *expanded;

// 	file_name = "./heredoc_tmp";
// 	fd = open(file_name, O_CREAT | O_TRUNC | O_WRONLY, 0644);
// 	if (fd < 0)
// 	{
// 		perror("heredoc fd error");
// 		return (0);
// 	}
// 	if (g_signal == SIGINT)
// 	{
// 		close(fd);
// 		unlink(file_name);
// 		write(STDOUT_FILENO, "\n", 1);
// 		return (0);
// 	}
// 	if (!cmd->info->env)
// 	{
// 		fprintf(stderr, " ERROR: info->env is NULL\n");
// 	}
// 	while (g_signal != SIGINT)
// 	{
// 		line = readline("> ");
// 		if (!line)
// 		{
// 			write(STDERR_FILENO, "warning: heredoc delimited by end-of-file\n",
// 				43);
// 			break ;
// 		}
// 		printf("line = %s\n", line);
// 		if (ft_strncmp(line, limiter, ft_strlen(limiter) + 1) == 0)
// 		{
// 			free(line);
// 			break ;
// 		}
// 		printf("test1\n");
// 		expanded = expand_env_in_heredoc_line(cmd->info, line);
// 		if(!expanded)
// 			expanded = ft_strdup(line);
// 		printf("expanded = %s\n", expanded);
//     	write(fd, expanded, ft_strlen(expanded));
// 		// write(fd, line, ft_strlen(line));
// 		write(fd, "\n", 1);
// 		free(line);
// 			// free(expanded);
// 	}
// 	close(fd);
// 	fd = open(file_name, O_RDONLY);
// 	if (fd < 0)
// 	{
// 		perror("heredoc fd error");
// 		return (0);
// 	}
// 	if (cmd->heredoc_str)
// 		free(cmd->heredoc_str);
// 	cmd->heredoc_str = ft_strdup("");
// 	if (!cmd->heredoc_str)
// 		return (0);
// 	while((gnl = get_next_line(fd)) != NULL) //Is this allowed by the norm?
// 	{
// 		temp = ft_strjoin(cmd->heredoc_str, gnl);
// 		free(cmd->heredoc_str);
// 		cmd->heredoc_str = temp;
// 		free(gnl);
// 		///temp EXECUTE에서 프리 아마도 룹돌리기
// 	}
// 	close(fd);
// 	unlink(file_name);
// 	if (cmd->heredoc_str)
// 		printf("HEREDOC CONTENT:%s", cmd->heredoc_str);
// 	return (1);
// }
