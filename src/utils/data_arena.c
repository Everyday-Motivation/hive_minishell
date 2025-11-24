/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_arena.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaeklee <jaeklee@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 16:57:41 by jaeklee           #+#    #+#             */
/*   Updated: 2025/11/24 15:48:19 by jaeklee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	arena_init(t_arena *arena)
{
	arena->block = ft_calloc(1, ARENA_INIT_SIZE);
	if (!arena->block)
		return (0);
	arena->size = 0;
	arena->capacity = ARENA_INIT_SIZE;
	arena->next = NULL;
	return (1);
}

void	arena_free(t_arena *arena)
{
	t_arena	*cur;
	t_arena	*next;

	cur = arena->next;
	while (cur != NULL)
	{
		next = cur->next;
		free(cur->block);
		free(cur);
		cur = next;
	}
	free(arena->block);
	arena->block = NULL;
	arena->size = 0;
	arena->capacity = 0;
	arena->next = NULL;
}

t_arena	*arena_new_block(size_t n)
{
	t_arena	*new_arena;
	size_t	new_capacity;

	new_capacity = ARENA_INIT_SIZE;
	while (new_capacity < n)
		new_capacity *= 2;
	new_arena = ft_calloc(1, sizeof(t_arena));
	if (!new_arena)
		return (NULL);
	new_arena->block = ft_calloc(1, new_capacity);
	if (!new_arena->block)
	{
		free(new_arena);
		return (NULL);
	}
	new_arena->size = 0;
	new_arena->capacity = new_capacity;
	new_arena->next = NULL;
	return (new_arena);
}

void	*arena_alloc(t_arena *arena, size_t n)
{
	t_arena	*curr;
	char	*ptr;
	t_arena	*new_arena;

	curr = arena;
	if (!arena)
		return (NULL);
	while (curr->next && curr->size + n > curr->capacity)
		curr = curr->next;
	if (curr->size + n > curr->capacity)
	{
		new_arena = arena_new_block(n);
		if (!new_arena)
			return (NULL);
		curr->next = new_arena;
		curr = new_arena;
	}
	ptr = curr->block + curr->size;
	curr->size += n;
	return ((void *)ptr);
}

char	*arena_strdup(t_arena *arena, const char *s, size_t n)
{
	char	*dest;

	if (!arena || !s)
	{
		return (NULL);
	}
	dest = arena_alloc(arena, n + 1);
	ft_memmove(dest, s, n);
	dest[n] = '\0';
	return (dest);
}
