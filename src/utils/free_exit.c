/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_exit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 19:40:00 by timurray          #+#    #+#             */
/*   Updated: 2025/12/02 10:21:38 by timurray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_exit(t_info *info, t_vec *cmds, int code)
{
	free_str_vec(info->env);
	if (cmds)
		free_cmd_vec(cmds);
	arena_free(info->arena);
	exit(code);
}
