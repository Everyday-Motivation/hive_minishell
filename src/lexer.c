/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 11:17:32 by timurray          #+#    #+#             */
/*   Updated: 2025/09/18 17:30:25 by timurray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void lexer(char *line, t_vec *lex_line)
{
	size_t	i;
	size_t	j;
	// char *word;
	t_lex elem;

	i = 0;
	ft_vec_new(lex_line,0,sizeof(t_lex));
	while(line[i])
	{
		while(ft_isspace(line[i]))
			i++;
		if(line[i]=='<'&&line[i+1]=='<')
		{
			printf("<<\n");
			i += 2;
		}
		if(line[i]=='<')
		{
			printf("<\n");
			i++;
		}
		if(line[i] == '\"')
		{
			j = i;
			while(line[j + 1] && (line[j +  1] != '\"'))
				j++;
			elem.type = WORD;
			elem.word = ft_substr(line, (i + 1), (j - i));
			ft_vec_push(lex_line, &elem);
			// free(word);
			i = j + 1;
		}
		i++;
	}
	ft_vec_free(lex_line);
}
