
#include "../include/minishell.h"

// char *expand_env(t_arena *arena, const char *input, t_vec *env)
// {
// 	t_vec parts;
// 	size_t i = 0, start = 0, total = 0;

// 	if (ft_vec_new(&parts, 0, sizeof(char *)) < 0)
// 		return NULL;

// 	while (input[i])
// 	{
// 		if (input[i] == '$' && (i == 0 || input[i - 1] != '\\'))
// 		{
// 			start = ++i;
// 			while (input[i] && (ft_isalnum(input[i]) || input[i] == '_'))
// 				i++;

// 			char *key = arena_strdup(arena, input + start, i - start);
// 			char *val = get_env_value(env, key);
// 			if (val)
// 			{
// 				char *frag = arena_strdup(arena, val, ft_strlen(val));
// 				ft_vec_push(&parts, &frag);
// 				total += ft_strlen(frag);
// 			}
// 		}
// 		else
// 		{
// 			char *frag = arena_strdup(arena, &input[i], 1);
// 			ft_vec_push(&parts, &frag);
// 			total += 1;
// 			i++;
// 		}
// 	}

// 	// arena에 최종 문자열 저장 위치 확보
// 	char *result = arena->block + arena->size;
// 	size_t j = 0;
// 	while (j < parts.len)
// 	{
// 		char *frag = *(char **)ft_vec_get(&parts, j);
// 		size_t k = 0;
// 		while (frag[k])
// 		{
// 			result[arena->size - (result - arena->block)] = frag[k];
// 			arena->size++;
// 			k++;
// 		}
// 		j++;
// 	}
// 	result[arena->size - (result - arena->block)] = '\0';
// 	arena->size++;

// 	ft_vec_free(&parts);
// 	return result;
// }

char *expand_env(t_arena *arena, const char *input, t_vec *env)
{
	t_vec parts;
	size_t i = 0, start = 0, total = 0;
	int in_single_quote = 0; // 싱글 쿼트 안인지 체크

	if (ft_vec_new(&parts, 0, sizeof(char *)) < 0)
		return NULL;

	while (input[i])
	{
		if (input[i] == '\'')  // 싱글 쿼트 시작/끝이면 상태 토글
		{
			in_single_quote = !in_single_quote;
			char *frag = arena_strdup(arena, &input[i], 1);
			ft_vec_push(&parts, &frag);
			total += 1;
			i++;
		}
		else if (input[i] == '$' && !in_single_quote && (i == 0 || input[i - 1] != '\\'))
		{
			start = ++i;
			while (input[i] && (ft_isalnum(input[i]) || input[i] == '_'))
				i++;

			char *key = arena_strdup(arena, input + start, i - start);
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

	// 결과 합치는 기존 코드 유지
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



char *get_env_value(t_vec *env, const char *var_name)
{
    size_t i = 0;
    char *entry;

    while (i < env->len)
    {
        entry = *(char **)ft_vec_get(env, i);
        if (strncmp(entry, var_name, strlen(var_name)) == 0 && entry[strlen(var_name)] == '=')
        {
            return entry + strlen(var_name) + 1;
        }
        i++;
    }
    return NULL;
}
