/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaeklee <jaeklee@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 12:51:29 by timurray          #+#    #+#             */
/*   Updated: 2025/09/19 13:57:25 by jaeklee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"


int	ft_isspace(char c)
{
	if ((c >= 9 && c <= 13) || (c == 32))
		return (1);
	return (0);
}

int	tokenizing(t_arena *arena,char *input)
{
	size_t	i;
	size_t	start;
	char	*token;
	char 	quote;

	i = 0;
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
			if ((input[i] == '>' && input[i + 1] == '>') ||
				(input[i] == '<' && input[i + 1] == '<'))
			{
				token = arena_strdup(arena, &input[i], 2);
				i += 2;
			}
			else
			{
				token = arena_strdup(arena, &input[i], 1);
				i++;
			}
			printf("[token] %s\n", token);
			continue;
		}
	
		
		start = i;
		while (input[i] && !(ft_isspace(input[i]) || deli_check(input[i])))
		{
			if (input[i] == '\'' || input[i] == '"')
			{
				quote = input[i];
				start = i + 1;
				i++;
				while (input[i] && input[i] != quote)
					i++;
				int len = i - start;  // 닫는 따옴표 전까지 길이
        		token = arena_strdup(arena, &input[start], len);
        		printf("[token] %s\n", token);
				if (input[i] == quote)
					i++;
			}
			else
				i++;
		}
		// int len = i - start;
		// token = arena_strdup(arena, &input[start],len -1);
		// printf("[token] %s\n", token);
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


