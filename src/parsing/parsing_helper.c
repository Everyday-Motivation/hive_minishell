/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_helper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaeklee <jaeklee@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 11:19:51 by jaeklee           #+#    #+#             */
/*   Updated: 2025/11/25 12:54:52 by jaeklee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_pipe(t_vec *tokens, t_token *tok, size_t *i)
{
	t_token	*next;

	if (tok->type == PIPE)
	{
		next = ft_vec_get(tokens, *i + 1);
		if (!next || next->type == PIPE)
		{
			return (-1);
		}
		(*i)++;
		return (EXIT_SUCCESS);
	}
	return (EXIT_FAILURE);
}

int	handle_ridir(t_vec *tokens, t_token *tok, size_t *i, t_cmd *cmd)
{
	t_token	*next;

	if (tok->type == S_LT || tok->type == S_GT || tok->type == D_LT
		|| tok->type == D_GT)
	{
		next = ft_vec_get(tokens, *i + 1);
		if (handle_redirection(cmd, tok, next) == EXIT_FAILURE)
			return (-1);
		(*i) += 2;
	}
	return (EXIT_SUCCESS);
}

int	open_heredoc_file_rdonly(char *file_name)
{
	int	fd;

	fd = open(file_name, O_RDONLY);
	if (fd < 0)
		ft_putendl_fd("heredoc fd error", 2);
	return (fd);
}

int	limiter_check(char *limiter)
{
	size_t	i;
	int		check;
	int		sign;

	sign = 0;
	i = 0;
	while (limiter[i] != '\0')
	{
		if (limiter[i] == '\'' || limiter[i] == '"')
		{
			sign = 1;
		}
		i++;
	}
	if (sign == 0)
		return (0);
	check = quote_check(limiter);
	if (check == 0)
		return (1);
	return (0);
}

size_t	handle_exit_status_variable(t_info *info, size_t *i, char **buf)
{
	char	*val;
	int		val_len;
	char	*temp;
	size_t	buf_len;

	(*i)++;
	val = ft_itoa(info->exit_code);
	val_len = ft_strlen(val);
	buf_len = ft_strlen(*buf);
	temp = arena_alloc(info->arena, buf_len + val_len + 1);
	if (!temp)
	{
		free(val);
		return (0);
	}
	ft_memcpy(temp, *buf, buf_len);
	ft_memcpy(temp + buf_len, val, val_len);
	temp[buf_len + val_len] = '\0';
	*buf = temp;
	free(val);
	return (val_len);
}
