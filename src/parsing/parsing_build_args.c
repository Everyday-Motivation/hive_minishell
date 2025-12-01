/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_build_args.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaeklee <jaeklee@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 17:59:37 by jaeklee           #+#    #+#             */
/*   Updated: 2025/12/01 18:07:30 by jaeklee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


static int	process_token(t_vec *tokens, size_t *i, t_cmd *cmd, t_token **out_tok)
{
	t_token	*tok;
	int		r;

	tok = ft_vec_get(tokens, *i);
	*out_tok = tok;

	if (handle_pipe(tokens, tok, i) == EXIT_SUCCESS)
		return 1;
	if (handle_pipe(tokens, tok, i) == -1)
		return -1;

	r = handle_ridir(tokens, tok, i, cmd);
	if (r == -2)
		return -2;
	if (r == -1)
		return -1;

	return 0;
}

// build_args 본문
char	**build_args(t_arena *arena, t_vec *tokens, size_t *i, t_cmd *cmd)
{
	char	**args;
	size_t	args_i;
	t_token	*tok;
	int		status;

	args_i = 0;
	args = arena_alloc(arena, sizeof(char *) * (count_word(tokens, *i) + 1));
	if (!args)
		return (NULL);

	while (*i < tokens->len)
	{
		status = process_token(tokens, i, cmd, &tok);
		if (status == 1)    // pipe 만남
			break ;
		if (status == -1)   // 일반 오류
			return (NULL);
		if (status == -2)   // heredoc Ctrl+C
			return (HD_INT);
		if (tok->type == WORD)
		{
			args[args_i] = tok->data;
			args_i++;
			(*i)++;
		}
	}

	args[args_i] = NULL;
	return (args);
}
