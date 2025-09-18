/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 11:17:32 by timurray          #+#    #+#             */
/*   Updated: 2025/09/18 13:26:45 by timurray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void lexer(char *line, t_vec *lex_line)
{
	size_t	i;
	size_t	j;
	// char *word;

	i = 0;
	// ft_vec_new(&lex_line,0,sizeof(t_lex));
	while(line[i])
	{
		while(ft_isspace(line[i]))
			i++;
		if(line[i]=='<'&&line[i+1]=='<')
		{
			printf("<<\n");
			i = i + 2;
		}
		if(line[i]=='<')
		{
			printf("<\n");
			i++;
		}
		if(line[i] == '\"')
		{
			j = i + 1;
			while(line[j] && (line[j] != '\"'))
				j++;
			i = i + j;
		}
		i++;
	}
	ft_vec_free(lex_line);
}
