/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 12:51:29 by timurray          #+#    #+#             */
/*   Updated: 2025/09/27 17:39:04 by timurray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*get_env_value(t_vec *env, const char *var_name)
{
	size_t	i;
	size_t	name_len;
	char	*entry;

	i = 0;
	if (!var_name || var_name[0] == '\0')
		return (NULL);
	name_len = strlen(var_name);
	while (i < env->len)
	{
		entry = *(char **)ft_vec_get(env, i);
		if (entry && strncmp(entry, var_name, name_len) == 0
			&& entry[name_len] == '=')
			return (entry + name_len + 1);
		i++;
	}
	return (NULL);
}

char	*expand_env(t_arena *arena, const char *input, t_vec *env)
{
	t_vec	parts;
	size_t	i = 0, start = 0, total;
	char	*key;
	char	*val;
	char	*frag;
	char	*result;
	size_t	j;

	size_t	k;

	i = 0, start = 0, total = 0;
	if (ft_vec_new(&parts, 0, sizeof(char *)) < 0)
		return (NULL);
	while (input[i])
	{
		if (input[i] == '$' && (i == 0 || input[i - 1] != '\\'))
		{
			start = ++i;
			while (input[i] && (ft_isalnum(input[i]) || input[i] == '_'))
				i++;
			key = arena_strdup(arena, input + start, i - start);
			val = get_env_value(env, key);
			if (val)
			{
				frag = arena_strdup(arena, val, ft_strlen(val));
				ft_vec_push(&parts, &frag);
				total += ft_strlen(frag);
			}
		}
		else
		{
			frag = arena_strdup(arena, &input[i], 1);
			ft_vec_push(&parts, &frag);
			total += 1;
			i++;
		}
	}
	result = arena->block + arena->size;
	j = 0;
	while (j < parts.len)
	{
		frag = *(char **)ft_vec_get(&parts, j);
		k = 0;
		while (frag[k])
		{
			result[arena->size - (result - arena->block)] = frag[k];
			arena->size++;
			k++;
		}
		j++;
	}
	result[arena->size - (result - arena->block)] = '\0';
	arena->size++;
	ft_vec_free(&parts);
	return (result);
}

int	tokenizing(t_arena *arena, char *input, t_vec *tokens, t_vec *env)
{
	size_t	i;
	size_t	start;
	char	quote;
	t_token	token;
	size_t	buf_i;
	char	*temp;
	size_t	var_start;
	char	*key;
	char	*val;

	i = 0;
	char buf[1024]; // 간단한 임시 버퍼
	if (ft_vec_new(tokens, 0, sizeof(token)) < 0)
		return (0);
	if (!quote_check(input, &i))
		return (0);
	i = 0;
	while (input[i])
	{
		if (ft_isspace(input[i]))
		{
			i++;
			continue ;
		}
		// 구분자 처리
		if (deli_check(input[i]))
		{
			if (input[i] == '<' && input[i + 1] == '<')
			{
				token.type = D_LT;
				token.data = arena_strdup(arena, &input[i], 2);
				i += 2;
			}
			else if (input[i] == '>' && input[i + 1] == '>')
			{
				token.type = D_GT;
				token.data = arena_strdup(arena, &input[i], 2);
				i += 2;
			}
			else if (input[i] == '<')
			{
				token.type = S_LT;
				token.data = arena_strdup(arena, &input[i], 1);
				i++;
			}
			else if (input[i] == '>')
			{
				token.type = S_GT;
				token.data = arena_strdup(arena, &input[i], 1);
				i++;
			}
			else if (input[i] == '|')
			{
				token.type = PIPE;
				token.data = arena_strdup(arena, &input[i], 1);
				i++;
			}
			ft_vec_push(tokens, &token);
			continue ;
		}
		// WORD 조합 처리 시작
		buf_i = 0;
		while (input[i] && !ft_isspace(input[i]) && !deli_check(input[i]))
		{
			if (input[i] == '\'') // 작은 따옴표
			{
				quote = input[i++];
				while (input[i] && input[i] != quote)
					buf[buf_i++] = input[i++];
				if (input[i] == quote)
					i++;
				// env 확장 없음
			}
			else if (input[i] == '"')
			{
				quote = input[i++];
				start = i;
				while (input[i] && input[i] != quote)
					i++;
				temp = arena_strdup(arena, &input[start], i - start);
				if (input[i] == quote)
					i++;
				temp = expand_env(arena, temp, env);
				for (size_t j = 0; temp[j]; j++)
					buf[buf_i++] = temp[j];
			}
			else // 일반 문자
			{
				if (input[i] == '$' && (i == 0 || input[i - 1] != '\\'))
				{
					var_start = ++i;
					while (input[i] && (ft_isalnum(input[i])
							|| input[i] == '_'))
						i++;
					key = arena_strdup(arena, &input[var_start], i - var_start);
					val = get_env_value(env, key);
					if (val)
					{
						for (size_t j = 0; val[j]; j++)
							buf[buf_i++] = val[j];
					}
				}
				else
				{
					buf[buf_i++] = input[i++];
				}
			}
		}
		buf[buf_i] = '\0';
		token.type = WORD;
		token.data = arena_strdup(arena, buf, buf_i);
		ft_vec_push(tokens, &token);
	}
	return (1);
}

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
