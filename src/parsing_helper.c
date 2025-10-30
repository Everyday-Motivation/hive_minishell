/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_helper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaeklee <jaeklee@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 10:49:09 by jaeklee           #+#    #+#             */
/*   Updated: 2025/10/30 10:49:38 by jaeklee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	handle_pipe(t_token *tok, size_t *i)
{
	if (tok->type == PIPE)
	{
		(*i)++;
		return (EXIT_SUCCESS);
	}
	return (EXIT_FAILURE);
}

int	handle_ridir(t_vec *tokens, t_token *tok, size_t *i, t_cmd *cmd)
{
	t_token	*next;

	if (tok->type == S_LT || tok->type == S_GT || tok->type == D_LT
		|| tok->type == D_GT)
	{
		next = ft_vec_get(tokens, *i + 1);
		if (handle_redirection(cmd, tok, next) == EXIT_FAILURE)
			return (-1);
		(*i) += 2;
	}
	return (EXIT_SUCCESS);
}
