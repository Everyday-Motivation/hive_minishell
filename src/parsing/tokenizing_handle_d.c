/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizing_handle_d.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaeklee <jaeklee@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 15:18:16 by jaeklee           #+#    #+#             */
/*   Updated: 2025/12/03 18:56:02 by jaeklee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	handle_double_quote_content(t_info *info, char *input, size_t *i,
		char **buf)
{
	size_t	buf_i;
	char	next;

	buf_i = ft_strlen(*buf);
	while (input[*i] && input[*i] != '"')
	{
		if (input[*i] == '$')
		{
			next = input[*i + 1];
			if (!(ft_isalnum(next) || next == '_' || next == '?'))
				(*buf)[buf_i++] = input[(*i)++];
			else
			{
				buf_i += handle_env_variable(info, input, i, buf);
			}
		}
		else
			(*buf)[buf_i++] = input[(*i)++];
	}
	if (input[*i] == '"')
		(*i)++;
	return (buf_i);
}

size_t	handle_double_quote(t_info *info, char *input, size_t *i, char **buf)
{
	char	quote;
	size_t	buf_i;

	quote = input[(*i)++];
	buf_i = ft_strlen(*buf);
	if (input[*i] == quote)
	{
		(*i)++;
		if (*buf == NULL || buf_i == 0)
		{
			(*buf)[buf_i] = '\0';
			return (1);
		}
		return (0);
	}
	return (handle_double_quote_content(info, input, i, buf));
}
