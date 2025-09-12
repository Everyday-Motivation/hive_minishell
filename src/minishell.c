/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 10:23:07 by timurray          #+#    #+#             */
/*   Updated: 2025/09/12 18:12:31 by timurray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	main(void)
{
	char	*line;

	while (1)
	{
		line = readline(">");
		if (!line)
		{
			ft_putendl_fd("Exiting..", 2);
			return (EXIT_FAILURE);
		}
		if (*line)
			add_history(line);
		free(line);
	}
	return (EXIT_SUCCESS);
}
