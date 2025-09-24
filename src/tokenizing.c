/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaeklee <jaeklee@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 12:51:29 by timurray          #+#    #+#             */
/*   Updated: 2025/09/24 17:12:47 by jaeklee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <fcntl.h>


int parse_tokens(t_arena *arena, t_vec *tokens, t_vec *cmds)
{
    t_cmd cmd;
    t_token *tok;
    t_vec argv;
    size_t i = 0;

    if (ft_vec_new(cmds, 0, sizeof(t_cmd)) < 0)
        return 0;

    while (i < tokens->len)
    {
        // 초기화
        cmd.input_fd = 0;
        cmd.output_fd = 1;
        cmd.heredoc = 0;
        ft_vec_new(&argv, 0, sizeof(char *));

        while (i < tokens->len)
        {
            tok = (t_token *)ft_vec_get(tokens, i);

            if (tok->type == PIPE)
            {
                i++; // 파이프 넘기기
                break;
            }
            else if (tok->type == S_LT || tok->type == S_GT || tok->type == D_LT || tok->type == D_GT)
            {
                // 리디렉션 처리
                t_token *next = ft_vec_get(tokens, i + 1);
                if (!next || next->type != WORD)
                    return 0; // 에러: 리디렉션 뒤에 파일 없음

                if (tok->type == S_LT)
                    cmd.input_fd = open(next->data, O_RDONLY);
                else if (tok->type == S_GT)
                    cmd.output_fd = open(next->data, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                else if (tok->type == D_GT)
                    cmd.output_fd = open(next->data, O_WRONLY | O_CREAT | O_APPEND, 0644);
                else if (tok->type == D_LT)
                {
                    cmd.heredoc = 1;
                    // heredoc 처리 필요 (나중에 구현)
                }

                i += 2;
            }
            else if (tok->type == WORD)
            {
                char *arg = arena_strdup(arena, tok->data, strlen(tok->data));
                ft_vec_push(&argv, &arg);
                i++;
            }
            else
            {
                i++; // 기타 토큰은 무시
            }
        }

        // NULL-terminated argv 만들기
        char **args = arena_alloc(arena, sizeof(char *) * (argv.len + 1));
        for (size_t j = 0; j < argv.len; j++)
            args[j] = *(char **)ft_vec_get(&argv, j);
        args[argv.len] = NULL;

        cmd.argv = args;
        ft_vec_push(cmds, &cmd);
    }

    return 1;
}

char *get_env_value(t_vec *env, const char *var_name)
{
    size_t i = 0;
    size_t name_len;
    char *entry;

    if (!var_name || var_name[0] == '\0')
        return NULL;

    name_len = strlen(var_name);

    while (i < env->len)
    {
        entry = *(char **)ft_vec_get(env, i);
        if (entry && strncmp(entry, var_name, name_len) == 0 && entry[name_len] == '=')
            return entry + name_len + 1;
        i++;
    }

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
			if (val)
			{

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
int tokenizing(t_arena *arena, char *input, t_vec *tokens, t_vec *env)
{
    size_t i = 0;
    size_t start;
    char quote;
    t_token token;

    if (ft_vec_new(tokens, 0, sizeof(token)) < 0)
        return 0;

    if (!quote_check(input, &i))
        return 0;

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
            ft_vec_push(tokens, &token);
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

        // 환경 변수 확장
        token.data = expand_env(arena, token.data, env);

        ft_vec_push(tokens, &token);
    }

    return 1;
}

// int	tokenizing(t_arena *arena, char *input, t_vec *env)
// {
// 	size_t	i = 0;
// 	size_t	start;
// 	char	quote;
// 	t_token	token;
// 	t_vec tokens;

// 	ft_vec_new(&tokens, 0, sizeof(token));

// 	if (!quote_check(input, &i))
// 		return (0);
// 	i = 0;

// 	while (input[i])
// 	{
// 		if (ft_isspace(input[i]))
// 		{
// 			i++;
// 			continue;
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
// 			ft_vec_push(&tokens, &token);
// 			// printf("[token] type=%d, data=%s\n", token.type, token.data);
// 			continue;
// 		}
// 		if (input[i] == '\'' || input[i] == '"')
// 		{
// 			quote = input[i];
// 			start = ++i;
// 			while (input[i] && input[i] != quote)
// 				i++;
// 			token.type = WORD;
// 			token.data = arena_strdup(arena, &input[start], i - start);
// 			if (input[i] == quote)
// 				i++;
// 		}
// 		else
// 		{
// 			start = i;
// 			while (input[i] && !(ft_isspace(input[i]) || deli_check(input[i])))
// 				i++;
// 			token.type = WORD;
// 			token.data = arena_strdup(arena, &input[start], i - start);
// 		}
// 		token.data = expand_env(arena, token.data, env);
// 		ft_vec_push(&tokens, &token);
// 		// printf("[token] type=%d, data=%s\n", token.type, token.data);
// 	}
// 	return (1);
// }


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


