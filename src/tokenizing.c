/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaeklee <jaeklee@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 12:51:29 by timurray          #+#    #+#             */
/*   Updated: 2025/09/23 12:35:31 by jaeklee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char *get_env_value(t_vec *env, const char *var_name)
{
    size_t i = 0;
    char *entry;
    size_t name_len;

    if (!var_name || var_name[0] == '\0')
        return NULL;

    name_len = strlen(var_name);
	while (i < env->len)
		{
			// ft_vec_get이 NULL을 반환하면 경고 출력
			void *ptr = ft_vec_get(env, i);
			if (!ptr)
			{
				printf("[WARN] ft_vec_get(env, %zu) returned NULL\n", i);
				i++;
				continue;
			}

			entry = *(char **)ptr;

			if (!entry)
			{
				printf("[WARN] env[%zu] entry is NULL\n", i);
				i++;
				continue;
			}

			// entry 전체 출력
			printf("[DEBUG] env[%zu] = \"%s\"\n", i, entry);

			// 환경변수 매칭
			if (strncmp(entry, var_name, name_len) == 0 && entry[name_len] == '=')
			{
				printf("[DEBUG] match found: returning \"%s\"\n", entry + name_len + 1);
				return entry + name_len + 1;
			}
			i++;
		}

		printf("[DEBUG] variable '%s' not found\n", var_name);
		return NULL;
}

char *expand_env(t_arena *arena, const char *input, t_vec *env)
{
	t_vec parts;
	size_t i = 0, start = 0, total = 0;
	char *key;

	if (ft_vec_new(&parts, 0, sizeof(char *)) < 0)
		return NULL;

	while (input[i])
	{
		if (input[i] == '$' && (i == 0 || input[i - 1] != '\\'))
		{
			start = ++i;
			while (input[i] && (ft_isalnum(input[i]) || input[i] == '_'))
				i++;

			key = arena_strdup(arena, input + start, i - start);
			char *val = get_env_value(env, key);
			if (!val)
			{
				printf("val error = NULL");
			}
			if (val)
			{
				printf("DEBUG: val='%s'\n", val);
				char *frag = arena_strdup(arena, val, ft_strlen(val));
				ft_vec_push(&parts, &frag);
				total += ft_strlen(frag);
			}
		}
		else
		{
			char *frag = arena_strdup(arena, &input[i], 1);
			ft_vec_push(&parts, &frag);
			total += 1;
			i++;
		}
	}
	// for (size_t j = 0; j < parts.len; j++)
	// {
	// 	char *frag = *(char **)ft_vec_get(&parts, j);
	// 	printf("[DEBUG] parts[%zu] = \"%s\"\n", j, frag);
	// }


	char *result = arena->block + arena->size;
	size_t j = 0;
	while (j < parts.len)
	{
		char *frag = *(char **)ft_vec_get(&parts, j);
		size_t k = 0;
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
	return result;
}






int	tokenizing(t_arena *arena, char *input, t_vec *env)
{
	size_t	i = 0;
	size_t	start;
	char	quote;
	t_token	token;
	t_vec tokens;

	ft_vec_new(&tokens, 0, sizeof(token));

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
			ft_vec_push(&tokens, &token);
			// printf("[token] type=%d, data=%s\n", token.type, token.data);
			continue;
		}
		if (input[i] == '\'' || input[i] == '"')
		{
			quote = input[i];
			start = ++i;
			while (input[i] && input[i] != quote)
				i++;
			token.type = WORD;
			token.data = arena_strdup(arena, &input[start], i - start);
			if (input[i] == quote)
				i++;
		}
		else
		{
			start = i;
			while (input[i] && !(ft_isspace(input[i]) || deli_check(input[i])))
				i++;
			token.type = WORD;
			token.data = arena_strdup(arena, &input[start], i - start);
		}
		token.data = expand_env(arena, token.data, env);
		ft_vec_push(&tokens, &token);
		// printf("[token] type=%d, data=%s\n", token.type, token.data);
	}
	return (1);
}


int quote_check(char *input, size_t *i)
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

int deli_check(char c)
{
	if (c == '|' || c == '<' || c == '>')
		return (1);
	return (0);
}


