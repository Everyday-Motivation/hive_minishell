/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaeklee <jaeklee@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 12:51:29 by timurray          #+#    #+#             */
/*   Updated: 2025/09/22 16:35:51 by jaeklee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	tokenizing(t_arena *arena, char *input)
{
	size_t	i = 0;
	size_t	start;
	char	quote;
	t_token	token;
	t_vec tokens;

	ft_vec_new(&tokens, 0, sizeof(token));

	if (!quote_check(input, &i))
		return (0);
	i = 0;

	while (input[i])
	{
		if (ft_isspace(input[i]))
		{
			i++;
			continue;
		}

		if (deli_check(input[i]))
		{
			if (input[i] == '<' && input[i + 1] == '<')
			{
				token.type = D_LT;
				token.data = arena_strdup(arena, &input[i], 2);
				i += 2;
			}
			else if (input[i] == '>' && input[i + 1] == '>')
			{
				token.type = D_GT;
				token.data = arena_strdup(arena, &input[i], 2);
				i += 2;
			}
			else if (input[i] == '<')
			{
				token.type = S_LT;
				token.data = arena_strdup(arena, &input[i], 1);
				i++;
			}
			else if (input[i] == '>')
			{
				token.type = S_GT;
				token.data = arena_strdup(arena, &input[i], 1);
				i++;
			}
			else if (input[i] == '|')
			{
				token.type = PIPE;
				token.data = arena_strdup(arena, &input[i], 1);
				i++;
			}
			ft_vec_push(&tokens, &token);
			// printf("[token] type=%d, data=%s\n", token.type, token.data);
			continue;
		}
		if (input[i] == '\'' || input[i] == '"')
		{
			quote = input[i];
			start = ++i;
			while (input[i] && input[i] != quote)
				i++;
			token.type = WORD;
			token.data = arena_strdup(arena, &input[start], i - start);
			if (input[i] == quote)
				i++;
		}
		else
		{
			start = i;
			while (input[i] && !(ft_isspace(input[i]) || deli_check(input[i])))
				i++;
			token.type = WORD;
			token.data = arena_strdup(arena, &input[start], i - start);
		}
		ft_vec_push(&tokens, &token);
		// printf("[token] type=%d, data=%s\n", token.type, token.data);
	}
	return (1);
}


int quote_check(char *input, size_t *i)
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

int deli_check(char c)
{
	if (c == '|' || c == '<' || c == '>')
		return (1);
	return (0);
}


