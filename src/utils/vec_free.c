/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_free.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 15:29:01 by timurray          #+#    #+#             */
/*   Updated: 2025/12/13 15:24:02 by timurray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_redirs(t_vec *redirs)
{
	size_t	i;
	t_redir	*redir;

	i = 0;
	while (i < redirs->len)
	{
		redir = (t_redir *)ft_vec_get(redirs, i);
		if (redir->type == D_LT && redir->data)
			free(redir->data);
		i++;
	}
	ft_vec_free(redirs);
}

void	free_cmd_vec(t_vec *cmds)
{
	size_t	i;
	t_cmd	*cmd;

	i = 0;
	while (i < cmds->len)
	{
		cmd = (t_cmd *)ft_vec_get(cmds, i);
		free_redirs(&cmd->redirs);
		i++;
	}
	ft_vec_free(cmds);
}
