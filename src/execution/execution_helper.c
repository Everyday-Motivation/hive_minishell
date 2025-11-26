/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_helper.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 12:58:29 by timurray          #+#    #+#             */
/*   Updated: 2025/11/26 18:12:31 by timurray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void close_pipes(int pipefd[2]) {
  close(pipefd[READ_END]);
  close(pipefd[WRITE_END]);
}

void close_used_pipes(int pipefd[3]) {
  if (pipefd[PREV_READ] != -1)
    close(pipefd[PREV_READ]);
  if (pipefd[READ_END] != -1)
    close(pipefd[READ_END]);
  if (pipefd[WRITE_END] != -1)
    close(pipefd[WRITE_END]);
}
