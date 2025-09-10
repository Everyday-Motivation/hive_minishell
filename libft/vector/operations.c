/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operations.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 10:14:06 by timurray          #+#    #+#             */
/*   Updated: 2025/09/09 18:03:16 by timurray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

int	ft_vec_push(t_vec *dst, void *src)
{
	if (!dst || !src)
		return (-1);
	else if (!dst->memory)
		ft_vec_new(dst, 1, dst->elem_size);
	if (dst->elem_size * dst->len >= dst->alloc_size)
	{
		if (ft_vec_resize(dst, dst->len * 2) < 0)
			return (-1);
	}
	ft_memcpy(&dst->memory[dst->elem_size * dst->len], src, dst->elem_size);
	dst->len = dst->len + 1;
	return (1);
}

int	ft_vec_push_front(t_vec *dst, void *src)
{
	if (!dst || !src)
		return (-1);
	else if (!dst->memory)
		ft_vec_new(dst, 1, dst->elem_size);
	if (dst->elem_size * dst->len >= dst->alloc_size)
	{
		if (ft_vec_resize(dst, dst->len * 2) < 0)
			return (-1);
	}
	if (dst->len > 0)
	{
		ft_memmove(dst->memory + dst->elem_size, dst->memory, dst->len
			* dst->elem_size);
	}
	ft_memcpy(dst->memory, src, dst->elem_size);
	dst->len += 1;
	return (1);
}

int	ft_vec_pop(void *dst, t_vec *src)
{
	if (!dst || !src)
		return (-1);
	else if (!src->memory || src->len == 0)
		return (0);
	ft_memcpy(dst, ft_vec_get(src, src->len - 1), src->elem_size);
	src->len--;
	return (1);
}

int	ft_vec_pop_front(void *dst, t_vec *src)
{
	if (!dst || !src)
		return (-1);
	else if (!src->memory || src->len == 0)
		return (0);
	ft_memcpy(dst, ft_vec_get(src, 0), src->elem_size);
	if (src->len > 1)
		ft_memmove(src->memory, src->memory + src->elem_size, (src->len - 1)
			* src->elem_size);
	src->len--;
	return (1);
}

void	*ft_vec_get(t_vec *src, size_t index)
{
	unsigned char	*v;

	if (!src || index >= src->len || !src->memory)
		return (NULL);
	v = &src->memory[src->elem_size * index];
	return (v);
}
