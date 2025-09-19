/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 11:17:32 by timurray          #+#    #+#             */
/*   Updated: 2025/09/19 12:08:40 by timurray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void print_v_lex(t_vec *lex)
{
	size_t i;
	t_lex *elem;

	i = 0;
	while (i < lex->len)
	{
		elem = ft_vec_get(lex,i);
		printf("type: %i\t data: %s\n", elem->type, elem->data);
		i++;
	}
}

void lexer(char *line, t_vec *lex_line)
{
	size_t	i;
	size_t	j;
	t_lex elem;

	i = 0;
	ft_vec_new(lex_line, 0, sizeof(t_lex));
	while(line[i])
	{
		while(ft_isspace(line[i]))
			i++;
		if((line[i] == '<' && line[i + 1] == '<') || (line[i] == '>' && line[i + 1] == '>'))
		{
			if(line[i]== '<')
			{
				elem.type = D_LT;
				elem.data = "<<";
			}
			else
			{
				elem.type = D_GT;
				elem.data = ">>";
			}
			ft_vec_push(lex_line, &elem);
			i += 2;
			continue;
		}
		if(line[i] == '<' || line[i] == '>')
		{
			if(line[i]== '<')
			{
				elem.type = S_LT;
				elem.data = "<";
			}
			else
			{
				elem.type = S_GT;
				elem.data = ">";
			}
			ft_vec_push(lex_line, &elem);
			i++;
			continue;
		}
		if(line[i] == '|')
		{
			elem.type = PIPE;
			elem.data = "|";
			ft_vec_push(lex_line, &elem);
			i++;
			continue;
		}
		if(line[i] == '\"')
		{
			j = i;
			while(line[j + 1] && (line[j +  1] != '\"'))
				j++;
			elem.type = D_Q;
			elem.data = ft_substr(line, (i + 1), (j - i));
			ft_vec_push(lex_line, &elem);
			i = j + 1;
			continue;
		}
		if(line[i] == '\'')
		{
			j = i;
			while(line[j + 1] && (line[j +  1] != '\''))
				j++;
			elem.type = S_Q;
			elem.data = ft_substr(line, (i + 1), (j - i));
			ft_vec_push(lex_line, &elem);
			i = j + 1;
			continue;
		}
		i++;
	}
	print_v_lex(lex_line);
	ft_vec_free(lex_line);
}

// Unclosed quotes aka ERR
// Single quotes - !interpret
// Double quotes - interpret $
// < redirect input
// > redirect output
// << needs delimeter. read docs.
// >> redirect in append mode.
//  | pipe
// $ expand
// $?
