/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_helper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 16:25:32 by jaeklee           #+#    #+#             */
/*   Updated: 2025/12/13 10:56:22 by timurray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_unlink_heredoc(int fd, char *file_name)
{
	close(fd);
	unlink(file_name);
}

char	*expand_env_in_heredoc_line(t_info *info, char *input)
{
	size_t	i;
	char	*buf;
	char	*temp;
	size_t	old_len;

	buf = arena_alloc(info->arena, ft_strlen(input));
	i = 0;
	while (input[i])
	{
		if (input[i] == '$')
			handle_env_variable(info, input, &i, &buf);
		else
		{
			old_len = ft_strlen(buf);
			temp = arena_alloc(info->arena, old_len + 2);
			ft_memmove(temp, buf, old_len);
			temp[old_len] = input[i++];
			temp[old_len + 1] = '\0';
			buf = temp;
		}
	}
	return (buf);
}

void	del_quotes(char *str)
{
	size_t	len;

	if (!str)
		return ;
	len = ft_strlen(str);
	while (len > 0 && (str[0] == '"' || str[0] == '\''))
	{
		ft_memmove(str, str + 1, len);
		len = ft_strlen(str);
	}
	while (len > 0 && (str[len - 1] == '"' || str[len - 1] == '\''))
	{
		str[len - 1] = '\0';
		len--;
	}
}
// int	handle_heredoc_signal(t_cmd *cmd, int fd, char *file_name)
int	handle_heredoc_signal(t_info *info, int fd, char *file_name)
{
	if (g_signal == SIGINT)
	{
		// cmd->info->exit_code = 130;
		info->exit_code = 130;
		close_unlink_heredoc(fd, file_name);
		init_signals();
		write(STDOUT_FILENO, "\n", 1);
		return (-2);
	}
	return (0);
}
