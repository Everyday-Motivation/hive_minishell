/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_free.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 15:29:01 by timurray          #+#    #+#             */
/*   Updated: 2025/12/13 10:42:56 by timurray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_cmd_vec(t_vec *cmds)
{
	size_t	i;
	size_t	j;
	t_cmd	*cmd;
	t_redir	*redir;

	i = 0;
	while (i < cmds->len)
	{
		cmd = (t_cmd *)ft_vec_get(cmds, i);
		j = 0;
		while (j < cmd->redirs.len)
		{
			redir = (t_redir *)ft_vec_get(&cmd->redirs, j);
			if (redir->type == D_LT && redir->data)
				free(redir->data);
			j++;
		}
		ft_vec_free(&cmd->redirs);
		i++;
	}
	ft_vec_free(cmds);
}

//TODO: seems good, double check