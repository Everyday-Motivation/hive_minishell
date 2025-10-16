/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaeklee <jaeklee@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 12:51:29 by timurray          #+#    #+#             */
/*   Updated: 2025/10/16 12:18:34 by jaeklee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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
		return (0);
	if (!quote_check(input, &i))
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
	return (1);
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
		perror("Syntax error: quotations need to match.\n");
		return (0);
	}
	return (1);
}

int	deli_check(char c)
{
	if (c == '|' || c == '<' || c == '>')
		return (1);
	return (0);
}
