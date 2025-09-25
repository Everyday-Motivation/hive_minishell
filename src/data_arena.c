/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_arena.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaeklee <jaeklee@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 16:57:41 by jaeklee           #+#    #+#             */
/*   Updated: 2025/09/25 15:07:40 by jaeklee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	arena_init(t_arena *arena)
{
	arena->block = malloc(ARENA_INIT_SIZE);
	if (!arena->block)
		return (0);
	arena->size = 0;
	arena->capacity = ARENA_INIT_SIZE;
	return (1);
}

void	arena_free(t_arena *arena)
{
	free(arena->block);
	arena->block = NULL;
	arena->size = 0;
	arena->capacity = 0;
}

char	*arena_strdup(t_arena *arena, const char *s, size_t n)
{
	char	*dest;
	char	*new_block;
	size_t	new_capacity;

	if (!arena || !s)
		return (NULL);
	if (arena->size + n + 1 > arena->capacity)
	{
		new_capacity = ARENA_INIT_SIZE;
		while (new_capacity < n + 1)
			new_capacity *= 2;
		new_block = malloc(new_capacity);
		if (!new_block)
		{
			arena->size = 0;
			arena->capacity = 0;
			return (NULL);
		}
		free(arena->block);
		arena->block = new_block;
		arena->capacity = new_capacity;
		arena->size = 0;
	}
	dest = arena->block + arena->size;
	ft_memcpy(dest, s, n);
	dest[n] = '\0';
	arena->size += n + 1;
	return (dest);
}

void *arena_alloc(t_arena *arena, size_t n)
{
	char *ptr;
	char *new_block;
	size_t new_capacity;

	if (!arena)
		return NULL;
	if (arena->size + n > arena->capacity)
	{
		new_capacity = arena->capacity * 2;
		while (new_capacity < arena->size + n)
			new_capacity *= 2;
		new_block = malloc(new_capacity);
		if (!new_block)
			return NULL;
		// Copy old contents to new block
		ft_memcpy(new_block, arena->block, arena->size);
		free(arena->block);
		arena->block = new_block;
		arena->capacity = new_capacity;
	}
	ptr = arena->block + arena->size;
	arena->size += n;
	return (void *)ptr;
}
