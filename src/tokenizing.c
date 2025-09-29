/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaeklee <jaeklee@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 12:51:29 by timurray          #+#    #+#             */
/*   Updated: 2025/09/29 13:33:41 by jaeklee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

size_t	handle_single_quote(char *input, size_t *i, char *buf, size_t buf_i)
{
	char quote = input[(*i)++];
	while (input[*i] && input[*i] != quote)
		buf[buf_i++] = input[(*i)++];
	if (input[*i] == quote)
		(*i)++;
	return buf_i;
}

size_t	handle_double_quote(t_arena *arena, char *input, size_t *i, char *buf, size_t buf_i, t_vec *env)
{
	char quote = input[(*i)++];
	size_t start = *i;
	while (input[*i] && input[*i] != quote)
		(*i)++;

	char *temp = arena_strdup(arena, &input[start], *i - start);

	if (input[*i] == quote)
		(*i)++;

	temp = expand_env(arena, temp, env);
	
	size_t j = 0;
	while (temp[j])
		buf[buf_i++] = temp[j++];

	return buf_i;
}

size_t	handle_env_variable(t_arena *arena, char *input, size_t *i, char *buf, size_t buf_i, t_vec *env)
{
	size_t var_start = ++(*i);
	while (input[*i] && (ft_isalnum(input[*i]) || input[*i] == '_'))
		(*i)++;

	char *key = arena_strdup(arena, &input[var_start], *i - var_start);
	char *val = get_env_value(env, key);

	if (val)
	{
		size_t j = 0;
		while (val[j])
			buf[buf_i++] = val[j++];
	}
	return buf_i;
}

void	process_word(t_arena *arena, char *input, size_t *i, t_vec *tokens, t_vec *env)
{
	char	buf[1024];
	size_t	buf_i = 0;
	t_token	token;

	while (input[*i] && !ft_isspace(input[*i]) && !deli_check(input[*i]))
	{
		if (input[*i] == '\'')
			buf_i = handle_single_quote(input, i, buf, buf_i);
		else if (input[*i] == '"')
			buf_i = handle_double_quote(arena, input, i, buf, buf_i, env);
		else
		{
			if (input[*i] == '$' && (*i == 0 || input[*i - 1] != '\\'))
				buf_i = handle_env_variable(arena, input, i, buf, buf_i, env);
			else
				buf[buf_i++] = input[(*i)++];
		}
	}
	buf[buf_i] = '\0';
	token.type = WORD;
	token.data = arena_strdup(arena, buf, buf_i);
	ft_vec_push(tokens, &token);
}

void	process_delimiter(t_arena *arena, char *input, size_t *i, t_vec *tokens)
{
	t_token token;

	if (input[*i] == '<' && input[*i + 1] == '<')
	{
		token.type = D_LT;
		token.data = arena_strdup(arena, &input[*i], 2);
		*i += 2;
	}
	else if (input[*i] == '>' && input[*i + 1] == '>')
	{
		token.type = D_GT;
		token.data = arena_strdup(arena, &input[*i], 2);
		*i += 2;
	}
	else if (input[*i] == '<')
	{
		token.type = S_LT;
		token.data = arena_strdup(arena, &input[*i], 1);
		(*i)++;
	}
	else if (input[*i] == '>')
	{
		token.type = S_GT;
		token.data = arena_strdup(arena, &input[*i], 1);
		(*i)++;
	}
	else if (input[*i] == '|')
	{
		token.type = PIPE;
		token.data = arena_strdup(arena, &input[*i], 1);
		(*i)++;
	}
	ft_vec_push(tokens, &token);
}

int	tokenizing(t_arena *arena, char *input, t_vec *tokens, t_vec *env)
{
	size_t	i;
	
	i = 0;
	if (ft_vec_new(tokens, 0, sizeof(t_token)) < 0)
		return (0);
	if (!quote_check(input, &i))
		return (0);
	i = 0;
	while (input[i])
	{
		if (ft_isspace(input[i]))
		{
			i++;
			continue;
		}
		if (deli_check(input[i]))
		{
			process_delimiter(arena, input, &i, tokens);
			continue;
		}
		process_word(arena, input, &i, tokens, env);
	}
	return (1);
}

int	quote_check(char *input, size_t *i)
{
	int	s_sign;
	int	d_sign;

	s_sign = 0;
	d_sign = 0;
	while (input[*i])
	{
		if (input[*i] == '\'' && d_sign == 0)
			s_sign = !s_sign;
		else if (input[*i] == '"' && s_sign == 0)
			d_sign = !d_sign;
		(*i)++;
	}
	if (d_sign == 1 || s_sign == 1)
	{
		perror("Syntax error: quotations need to match.\n");
		return (0);
	}
	return (1);
}

int	deli_check(char c)
{
	if (c == '|' || c == '<' || c == '>')
		return (1);
	return (0);
}


// int	tokenizing(t_arena *arena, char *input, t_vec *tokens, t_vec *env)
// {
// 	size_t	i;
// 	size_t	start;
// 	char	quote;
// 	t_token	token;
// 	size_t	buf_i;
// 	char	*temp;
// 	size_t	var_start;
// 	char	*key;
// 	char	*val;

// 	i = 0;
// 	char buf[1024];
// 	if (ft_vec_new(tokens, 0, sizeof(token)) < 0)
// 		return (0);
// 	if (!quote_check(input, &i))
// 		return (0);
// 	i = 0;
// 	while (input[i])
// 	{
// 		if (ft_isspace(input[i]))
// 		{
// 			i++;
// 			continue ;
// 		}
// 		if (deli_check(input[i]))
// 		{
// 			if (input[i] == '<' && input[i + 1] == '<')
// 			{
// 				token.type = D_LT;
// 				token.data = arena_strdup(arena, &input[i], 2);
// 				i += 2;
// 			}
// 			else if (input[i] == '>' && input[i + 1] == '>')
// 			{
// 				token.type = D_GT;
// 				token.data = arena_strdup(arena, &input[i], 2);
// 				i += 2;
// 			}
// 			else if (input[i] == '<')
// 			{
// 				token.type = S_LT;
// 				token.data = arena_strdup(arena, &input[i], 1);
// 				i++;
// 			}
// 			else if (input[i] == '>')
// 			{
// 				token.type = S_GT;
// 				token.data = arena_strdup(arena, &input[i], 1);
// 				i++;
// 			}
// 			else if (input[i] == '|')
// 			{
// 				token.type = PIPE;
// 				token.data = arena_strdup(arena, &input[i], 1);
// 				i++;
// 			}
// 			ft_vec_push(tokens, &token);
// 			continue ;
// 		}
// 		// WORD 조합 처리 시작
// 		buf_i = 0;
// 		while (input[i] && !ft_isspace(input[i]) && !deli_check(input[i]))
// 		{
// 			if (input[i] == '\'') // 작은 따옴표
// 			{
// 				quote = input[i++];
// 				while (input[i] && input[i] != quote)
// 					buf[buf_i++] = input[i++];
// 				if (input[i] == quote)
// 					i++;
// 				// env 확장 없음
// 			}
// 			else if (input[i] == '"')
// 			{
// 				quote = input[i++];
// 				start = i;
// 				while (input[i] && input[i] != quote)
// 					i++;
// 				temp = arena_strdup(arena, &input[start], i - start);
// 				if (input[i] == quote)
// 					i++;
// 				temp = expand_env(arena, temp, env);
// 				for (size_t j = 0; temp[j]; j++)
// 					buf[buf_i++] = temp[j];
// 			}
// 			else
// 			{
// 				if (input[i] == '$' && (i == 0 || input[i - 1] != '\\'))
// 				{
// 					var_start = ++i;
// 					while (input[i] && (ft_isalnum(input[i])
// 							|| input[i] == '_'))
// 						i++;
// 					key = arena_strdup(arena, &input[var_start], i - var_start);
// 					val = get_env_value(env, key);
// 					if (val)
// 					{
// 						for (size_t j = 0; val[j]; j++)
// 							buf[buf_i++] = val[j];
// 					}
// 				}
// 				else
// 				{
// 					buf[buf_i++] = input[i++];
// 				}
// 			}
// 		}
// 		buf[buf_i] = '\0';
// 		token.type = WORD;
// 		token.data = arena_strdup(arena, buf, buf_i);
// 		ft_vec_push(tokens, &token);
// 	}
// 	return (1);
// }

// int tokenizing(t_arena *arena, char *input, t_vec *tokens, t_vec *env)
// {
//     size_t i = 0;
//     size_t start;
//     char quote;
//     t_token token;

//     if (ft_vec_new(tokens, 0, sizeof(token)) < 0)
//         return (0);

//     if (!quote_check(input, &i))
//         return (0);

//     i = 0;
//     while (input[i])
//     {
//         if (ft_isspace(input[i]))
//         {
//             i++;
//             continue ;
//         }
//         if (deli_check(input[i]))
//         {
//             if (input[i] == '<' && input[i + 1] == '<')
//             {
//                 token.type = D_LT;
//                 token.data = arena_strdup(arena, &input[i], 2);
//                 i += 2;
//             }
//             else if (input[i] == '>' && input[i + 1] == '>')
//             {
//                 token.type = D_GT;
//                 token.data = arena_strdup(arena, &input[i], 2);
//                 i += 2;
//             }
//             else if (input[i] == '<')
//             {
//                 token.type = S_LT;
//                 token.data = arena_strdup(arena, &input[i], 1);
//                 i++;
//             }
//             else if (input[i] == '>')
//             {
//                 token.type = S_GT;
//                 token.data = arena_strdup(arena, &input[i], 1);
//                 i++;
//             }
//             else if (input[i] == '|')
//             {
//                 token.type = PIPE;
//                 token.data = arena_strdup(arena, &input[i], 1);
//                 i++;
//             }
//             ft_vec_push(tokens, &token);
//             continue ;
//         }
// 		if (input[i] == '\'')
// 		{
// 			quote = input[i];
// 			start = ++i;
// 			while (input[i] && input[i] != quote)
// 				i++;
// 			token.type = WORD;
// 			token.data = arena_strdup(arena, &input[start], i - start);
// 			if (input[i] == quote)
// 				i++;
// 			// No env_exp if It's S_Q
// 		}
// 		else if (input[i] == '"')
// 		{
// 			quote = input[i];
// 			start = ++i;
// 			while (input[i] && input[i] != quote)
// 				i++;
// 			token.type = WORD;
// 			token.data = arena_strdup(arena, &input[start], i - start);
// 			if (input[i] == quote)
// 				i++;
// 			token.data = expand_env(arena, token.data, env);
// 		}
// 		else
// 		{
// 			start = i;
// 			while (input[i] && !(ft_isspace(input[i]) || deli_check(input[i])))
// 				i++;
// 			token.type = WORD;
// 			token.data = arena_strdup(arena, &input[start], i - start);

// 			token.data = expand_env(arena, token.data, env);
// 		}
//         ft_vec_push(tokens, &token);
//     }
//     return (1);
// }

