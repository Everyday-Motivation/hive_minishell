/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_exp.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaeklee <jaeklee@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 12:43:41 by jaeklee           #+#    #+#             */
/*   Updated: 2025/10/22 18:04:52 by jaeklee          ###   ########.fr       */
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
	printf("env1\n");
	name_len = ft_strlen(var_name);

	while (i < env->len)
	{
		entry = *(char **)ft_vec_get(env, i);
		// printf("env3\n");
		// printf("entry = %s\n", entry);
		if (entry && strncmp(entry, var_name, name_len) == 0
			&& entry[name_len] == '=')
			return (entry + name_len + 1);
		i++;
	}

	return (NULL);
}

char *join_fragments_to_arena(t_vec *parts, t_arena *arena)
{
    size_t start = arena->size;  // 현재 위치 저장해놓기 (중요!)
    size_t j = 0;
    while (j < parts->len)
    {
        char *frag = *(char **)ft_vec_get(parts, j);
        size_t k = 0;
        while (frag[k])
            arena->block[arena->size++] = frag[k++];
        j++;
    }
    arena->block[arena->size++] = '\0';  // 널 종료

    return arena->block + start;  // 시작 위치 리턴해야 함
}


// char	*join_fragments_to_arena(t_vec *parts, t_arena *arena)
// {
// 	char	*result;
// 	size_t	j;
// 	char	*frag;
// 	size_t	k;

// 	result = arena->block + arena->size;
// 	j = 0;
// 	while (j < parts->len)
// 	{
// 		frag = *(char **)ft_vec_get(parts, j);
// 		k = 0;
// 		while (frag[k])
// 			arena->block[arena->size++] = frag[k++];
// 		j++;
// 	}
// 	arena->block[arena->size++] = '\0';
// 	return (result);
// }

char	*expand_env(t_arena *arena, const char *input, t_vec *env)
{
	t_vec	parts;
	size_t	i;
	size_t	start;
	char	*key;
	char	*val;

	i = 0;
	start = 0;
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
			if (!key)
			{
				printf("key = %s\n", key);
			}

			val = get_env_value(env, key);
			if (val)
				ft_vec_push(&parts, &val);
		}
		else
		{
			val = arena_strdup(arena, &input[i], 1);
			ft_vec_push(&parts, &val);
			i++;
		}
	}
	val = join_fragments_to_arena(&parts, arena);
	ft_vec_free(&parts);
	return (val);
}

// char *expand_env(t_arena *arena, const char *input, t_vec *env)
// {
// 	t_vec parts;
// 	size_t i = 0, start = 0, total = 0;
// 	char *key;
// 	char *frag;

// 	if (ft_vec_new(&parts, 0, sizeof(char *)) < 0)
// 		return (NULL);

// 	while (input[i])
// 	{
// 		if (input[i] == '$' && (i == 0 || input[i - 1] != '\\'))
// 		{
// 			start = ++i;
// 			while (input[i] && (ft_isalnum(input[i]) || input[i] == '_'))
// 				i++;

// 			key = arena_strdup(arena, input + start, i - start);
// 			char *val = get_env_value(env, key);
// 			if (val)
// 			{
// 				// char *frag = arena_strdup(arena, val, ft_strlen(val));
// 				ft_vec_push(&parts, &val);
// 				total += ft_strlen(val);
// 			}
// 		}
// 		else
// 		{
// 			frag = arena_strdup(arena, &input[i], 1);
// 			ft_vec_push(&parts, &frag);
// 			total += 1;
// 			i++;
// 		}
// 	}
// 	char *result = join_fragments_to_arena(&parts, arena);
// 	ft_vec_free(&parts);
// 	return (result);
// }
