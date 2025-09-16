/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 12:51:29 by timurray          #+#    #+#             */
/*   Updated: 2025/09/16 16:59:21 by timurray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	tokenizing(char *input)
{
	char	**token;
	size_t	i;

	i = 0;
	token = ft_split(input, ' ');
	while (input[i])
	{
		if ((input[i] == ' ' || input[i] == '\t' || input[i] == '\n'))
			i++;
	}
	return (0);
}

int	quote_check(char *input, size_t i)
{
	int	s_sign;
	int	d_sign;

	s_sign = 0;
	d_sign = 0;
	if (input[i] == '\'' && s_sign == 0)
	{
		s_sign = 1;
		i++;
	}
	if (input[i] == '"' && d_sign == 0)
	{
		d_sign = 1;
		i++;
	}
	if (input[i] == '\'' && s_sign == 1)
	{
		s_sign = 0;
		i++;
	}
	if (input[i] == '"' && d_sign == 1)
	{
		d_sign = 0;
		i++;
	}
	if (d_sign == 1 || s_sign == 1)
	{
		perror("Syntax error quotations need to match.");
	}
	return (EXIT_SUCCESS);
}

// int deli_check(char **input, size_t i)
// {

// }
