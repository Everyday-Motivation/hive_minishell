/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 11:17:32 by timurray          #+#    #+#             */
/*   Updated: 2025/09/24 10:10:09 by timurray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	print_v_lex(t_vec *lex)
{
	size_t	i;
	t_lex	*elem;

	i = 0;
	while (i < lex->len)
	{
		elem = ft_vec_get(lex, i);
		printf("type: %i\t data: %s\n", elem->type, elem->data);
		i++;
	}
}

void	lexer(char *line, t_vec *lex_line)
{
	size_t	i;
	size_t	j;
	t_lex	elem;

	i = 0;
	ft_vec_new(lex_line, 0, sizeof(t_lex));
	while (line[i])
	{
		while (ft_isspace(line[i]))
			i++;
		if ((line[i] == '<' && line[i + 1] == '<') || (line[i] == '>' && line[i
				+ 1] == '>'))
		{
			if (line[i] == '<')
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
			continue ;
		}
		if (line[i] == '<' || line[i] == '>')
		{
			if (line[i] == '<')
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
			continue ;
		}
		if (line[i] == '|')
		{
			elem.type = PIPE;
			elem.data = "|";
			ft_vec_push(lex_line, &elem);
			i++;
			continue ;
		}
		if (line[i] == '\"')
		{
			j = i;
			while (line[j + 1] && (line[j + 1] != '\"'))
				j++;
			elem.type = WORD;
			elem.data = ft_substr(line, (i + 1), (j - i));
			ft_vec_push(lex_line, &elem);
			i = j + 1;
			continue ;
		}
		if (line[i] == '\'')
		{
			j = i;
			while (line[j + 1] && (line[j + 1] != '\''))
				j++;
			elem.type = WORD;
			elem.data = ft_substr(line, (i + 1), (j - i));
			ft_vec_push(lex_line, &elem);
			i = j + 1;
			continue ;
		}
		j = i;
		while (line[j] && !ft_isspace(line[j]) && line[j] != '<'
			&& line[j] != '>' && line[j] != '|' && line[j] != '\"'
			&& line[j] != '\'')
			j++;
		if (j > i)
		{
			elem.type = WORD;
			elem.data = ft_substr(line, i, j - i);
			ft_vec_push(lex_line, &elem);
			i = j;
			continue ;
		}
		i++;
	}
	// print_v_lex(lex_line);
	// ft_vec_free(lex_line);
}

int	get_precedence(t_type t)
{
	if (t == S_LT || t == D_LT || t == S_GT || t == D_GT)
		return (20);
	if (t == PIPE)
		return (10);
	return (-1);
}

static t_lex	*get_lex(t_vec *lex, size_t i)
{
	return (t_lex *)(ft_vec_get(lex, i));
}

int	parse_command(t_vec *lex, size_t *i, t_cmd *cmd)
{
	t_lex	*t;
	t_lex	*op;
	t_type	kind;
	t_lex	*target;
	char	*s;
	t_redir	r;

	if (ft_vec_new(&cmd->args, 0, sizeof(char *)) < 0)
		return (0);
	if (ft_vec_new(&cmd->redirs, 0, sizeof(t_redir)) < 0)
		return (0);
	while (1)
	{
		t = get_lex(lex, *i);
		if (!t)
			break ;
		if (!(t->type == WORD))
			break ;
		s = t->data;
		if (ft_vec_push(&cmd->args, &s) < 0)
			return (0);
		(*i)++;
	}
	while (1)
	{
		op = get_lex(lex, *i);
		if (!op)
			break ;
		if (!(op->type == S_LT || op->type == D_LT || op->type == S_GT
				|| op->type == D_GT))
			break ;
		kind = op->type;
		(*i)++;
		target = get_lex(lex, *i);
		if (!target || !(target->type == WORD))
		{
			// TODO: syntax error
			// TODO: free cmd
			return (0);
		}
		r.type = kind;
		r.data = target->data;
		if (ft_vec_push(&cmd->redirs, &r) < 0)
			return (0);
		(*i)++;
	}
	return (1);
}

int	precedence_climb(t_vec *lex, size_t *i, int min_prec, t_vec *plan)
{
	t_cmd	in_cmd;
	t_task	left_task;
	t_lex	*op;
	int		prec;
	int		right_min_prec;
	t_type	op_kind;
	t_task	top;

	if (!parse_command(lex, i, &in_cmd))
		return (0);
	left_task.action_type = CMD;
	left_task.u_action.cmd = in_cmd;
	if (ft_vec_push(plan, &left_task) < 0)
		return (0);
	while (1)
	{
		op = get_lex(lex, *i);
		if (!op)
			break ;
		prec = get_precedence(op->type);
		if (prec < min_prec)
			break ;
		right_min_prec = prec + 1;
		op_kind = op->type;
		(*i)++;
		if (!precedence_climb(lex, i, right_min_prec, plan))
			return (0);
		top.action_type = OP;
		top.u_action.op = op_kind;
		if (ft_vec_push(plan, &top) < 0)
			return (0);
	}
	return (1);
}

int	lex_to_parse(t_vec *lex, t_vec *parse)
{
	size_t	i;

	i = 0;
	if (ft_vec_new(parse, 0, sizeof(t_task)) < 0)
		return (0);
	if (!precedence_climb(lex, &i, 0, parse))
		return (0);
	if (i != lex->len)
		return (0);
	return (1);
}
