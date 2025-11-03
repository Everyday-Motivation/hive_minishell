/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaeklee <jaeklee@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 12:51:29 by timurray          #+#    #+#             */
/*   Updated: 2025/11/03 17:19:13 by jaeklee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	process_delimiter(t_info *info, char *input, size_t *i, t_vec *tokens)
{
	t_token	token;
	int		len;

	len = 1;
	if (input[*i] == '<' && input[*i + 1] == '<')
	{
		token.type = D_LT;
		len = 2;
	}
	else if (input[*i] == '>' && input[*i + 1] == '>')
	{
		token.type = D_GT;
		len = 2;
	}
	else if (input[*i] == '<')
		token.type = S_LT;
	else if (input[*i] == '>')
		token.type = S_GT;
	else if (input[*i] == '|')
		token.type = PIPE;
	else
		return ;
	token.data = arena_strdup(info->arena, &input[*i], len);
	*i += len;
	ft_vec_push(tokens, &token);
}

int	tokenizing(t_info *info, char *input, t_vec *tokens)
{
	size_t	i;

	i = 0;
	if (ft_vec_new(tokens, 0, sizeof(t_token)) < 0)
		return (EXIT_FAILURE);
	if (quote_check(input, &i) == EXIT_FAILURE)
		return (0);
	i = 0;
	while (input[i])
	{
		if (ft_isspace(input[i]))
		{
			i++;
			continue ;
		}
		if (deli_check(input[i]))
		{
			process_delimiter(info, input, &i, tokens);
			continue ;
		}
		process_word(info, input, &i, tokens);
	}
	return (EXIT_SUCCESS);
}

int	quote_check(char *input, size_t *i)
{
	int	s_sign;
	int	d_sign;

	s_sign = 0;
	d_sign = 0;
	while (input[*i])
	{
		if (input[*i] == '\'' && d_sign == 0)
			s_sign = !s_sign;
		else if (input[*i] == '"' && s_sign == 0)
			d_sign = !d_sign;
		(*i)++;
	}
	if (d_sign == 1 || s_sign == 1)
	{
		ft_putendl_fd("Syntax error: quotations need to match.\n", 2);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
//&& d_sign == 0 → 현재 큰따옴표 안이면 작은따옴표는 토글하지 말라는 의미
//반대로 && s_sign == 0 → 현재 작은따옴표 안이면 큰따옴표는 토글하지 말라는 의미

int	deli_check(char c)
{
	if (c == '|' || c == '<' || c == '>')
		return (1);
	return (0);
}
