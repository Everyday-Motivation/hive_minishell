/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 10:23:07 by timurray          #+#    #+#             */
/*   Updated: 2025/09/16 19:22:02 by timurray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static char *read_line(int interactive)
{
	char *prompt;
    char *line;

    if (interactive)
	{
        prompt = get_prompt();
        line = readline(prompt);
        free(prompt);
        if (!line)
            exit_clear_rl_history();
        if (*line)
            add_history(line);
        return (line);
    }
	else
	{
        line = get_next_line(STDIN_FILENO);
        if (!line)
            return (NULL);
        size_t len = ft_strlen(line);
        if (len && line[len - 1] == '\n')
            line[len - 1] = '\0';
        return (line);
    }
}

static void shell_loop(int interactive)
{
    char *line;

    while (1) 
	{
        line = read_line(interactive);
        if (!line)
            break ;
        printf("read line: %s\n", line);
        //TODO: execute_line(line);
        free(line);
    }
}

int main(int ac, char **av, char **envp)
{
    (void)av;
	(void)envp;
    if (ac != 1)
	{
        ft_putendl_fd("minishell cannot execute binaries.", 2);
        return (1);
    }
    init_signals();
    shell_loop(isatty(STDIN_FILENO));
    return (0);
}
