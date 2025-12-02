/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_signal.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaeklee <jaeklee@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 15:48:42 by timurray          #+#    #+#             */
/*   Updated: 2025/12/02 15:17:51 by jaeklee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	parent_sig(void)
{
	// signal(SIGINT, SIG_IGN);
	// signal(SIGQUIT, SIG_IGN);
    struct sigaction sa;
    ft_bzero(&sa, sizeof(sa));
    sa.sa_handler = sigint_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sigaction(SIGINT, &sa, NULL);
    signal(SIGQUIT, SIG_IGN);
}
// Modified for signal issue control + C

void	child_sig(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}
