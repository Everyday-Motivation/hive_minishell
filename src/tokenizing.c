/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaeklee <jaeklee@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 12:51:29 by timurray          #+#    #+#             */
/*   Updated: 2025/09/19 12:35:52 by jaeklee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	tokenizing(t_arena *arena,char *input)
{
	size_t	i;
	size_t	start;
	char	*token;

	// char	**token;
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
				token = arena_strdup(arena, &input[i]);
				i += 2;
			}
			else
			{
				token = arena_strdup(arena, &input[i]);
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
				if (!quote_check(input, i))
					return (0);
				continue;
			}
			i++;
		}
		token = arena_strdup(arena, &input[start]);
		printf("[token] %s\n", token);
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


