/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaeklee <jaeklee@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 13:18:49 by jaeklee           #+#    #+#             */
/*   Updated: 2025/11/03 17:44:31 by jaeklee          ###   ########.fr       */
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

int limiter_check(char *limiter)
{
	size_t i;
	int check;
	int sign;

	printf("limit1 = %s\n", limiter);
	sign = 0;
	i = 0;
	while(limiter[i] != '\0')
	{
		if (limiter[i] == '\'' || limiter[i] == '"')
		{
			sign = 1;
		}
		i++;
	}
	if (sign == 0)
		return (0);
	check = quote_check(limiter, &i);
	if (check == 0)
		return (1);
	return (0);
}

static int	open_heredoc_file(char *file_name)
{
	int	fd;

	fd = open(file_name, O_CREAT | O_TRUNC | O_WRONLY, 0644);
	if (fd < 0)
		perror("heredoc fd error");
	return (fd);
}

static int	write_heredoc_input(t_cmd *cmd, int fd, char *limiter, int quote_flag)
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
		{
			expanded = expand_env_in_heredoc_line(cmd->info, line);
		}
		if (!expanded)
			expanded = ft_strdup(line);
		write(fd, expanded, ft_strlen(expanded));
		write(fd, "\n", 1);
		free(line);
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
//AFRER HEREDOC = free(temp), free(result); 

int	handle_heredoc(t_cmd *cmd, t_token *limiter)
{
	char	*file_name;
	int		fd;
	int 	quote_flag;

	quote_flag = limiter_check(limiter->raw_data);
	printf("flag = %d\n", quote_flag);
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
	write_heredoc_input(cmd, fd, limiter->data, quote_flag);
	close(fd);
	printf("heredoc_str = %s\n", cmd->heredoc_str);
	if (cmd->heredoc_str)
		free(cmd->heredoc_str);
	cmd->heredoc_str = read_heredoc_content(file_name);
	unlink(file_name);
	if (!cmd->heredoc_str)
		return (0);
	printf("HEREDOC CONTENT:%s", cmd->heredoc_str);
	return (1);
}

// int	handle_heredoc(t_cmd *cmd, char *limiter)
// {
// 	char	*line;
// 	int		fd;
// 	char	*file_name;
// 	char	*gnl;
// 	char	*temp;
//     char    *expanded;
// 	bool	limit_quote_check;

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
// 	limit_quote_check = limiter_check(limiter);
// 	printf("limit_test = %d\n", limit_quote_check);
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
// 		if (limit_quote_check != 0)
// 		{
// 			expanded = expand_env_in_heredoc_line(cmd->info, line);
// 		}
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
