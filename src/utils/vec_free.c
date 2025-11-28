/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_free.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 15:29:01 by timurray          #+#    #+#             */
/*   Updated: 2025/11/28 15:29:27 by timurray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_cmd_vec(t_vec *cmds)
{
	size_t i;
	t_cmd *cmd;

	i = 0;
	while (i < cmds->len)
	{
		cmd = (t_cmd *)ft_vec_get(cmds, i);
		if (cmd->heredoc_str)
			free(cmd->heredoc_str);
		i++;
	}
	ft_vec_free(cmds);
}
