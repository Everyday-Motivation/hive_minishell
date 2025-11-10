/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_helper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaeklee <jaeklee@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 16:25:32 by jaeklee           #+#    #+#             */
/*   Updated: 2025/11/10 12:09:15 by jaeklee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_unlink_heredoc(int fd, char *file_name)
{
	close(fd);
	unlink(file_name);
}

size_t	double_quote_heredoc(t_info *info, char *input, size_t *i, char **buf)
{
	char	quote;
	size_t	buf_i;

	quote = input[(*i)++];
	buf_i = ft_strlen(*buf);
	(*buf)[buf_i++] = quote;
	while (input[*i] && input[*i] != quote)
	{
		if (input[*i] == '$' && input[*i + 1])
			buf_i += handle_env_variable(info, input, i, buf);
		else
			(*buf)[buf_i++] = input[(*i)++];
	}
	if (input[*i] == quote)
		(*buf)[buf_i++] = input[(*i)++];
	return (buf_i);
}
