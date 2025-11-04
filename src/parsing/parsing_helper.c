/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_helper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaeklee <jaeklee@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 11:19:51 by jaeklee           #+#    #+#             */
/*   Updated: 2025/11/04 12:50:47 by jaeklee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	handle_pipe(t_token *tok, size_t *i)
{
	if (tok->type == PIPE)
	{
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

int	open_heredoc_file_rdonly(char *file_name)
{
	int	fd;

	fd = open(file_name, O_RDONLY);
	if (fd < 0)
		perror("heredoc fd error");
	return (fd);
}

int	limiter_check(char *limiter)
{
	size_t	i;
	int		check;
	int		sign;

	printf("limit1 = %s\n", limiter);
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
	check = quote_check(limiter, &i);
	if (check == 0)
		return (1);
	return (0);
}
