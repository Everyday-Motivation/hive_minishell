/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_helper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaeklee <jaeklee@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 16:25:32 by jaeklee           #+#    #+#             */
/*   Updated: 2025/11/11 13:07:10 by jaeklee          ###   ########.fr       */
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
