/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_char_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 15:47:01 by timurray          #+#    #+#             */
/*   Updated: 2025/08/18 14:47:13 by timurray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

ssize_t	ft_str_len(const char *s)
{
	ssize_t	length;

	length = 0;
	while (*s++)
		length += 1;
	return (length);
}

int	ft_putchar_fd(const char c, const int fd)
{
	int	res;

	res = (write(fd, &c, 1));
	if (res == -1)
		return (-1);
	return (1);
}

ssize_t	ft_putstr_fd(const char *s, const int fd)
{
	ssize_t	count;
	ssize_t	res;

	count = 0;
	res = 0;
	while (*s)
	{
		res = ft_putchar_fd(*s, fd);
		if (res == -1)
			return (-1);
		count += res;
		s++;
	}
	return (count);
}

int	char_handler(const int c)
{
	return (ft_putchar_fd(c, 1));
}

ssize_t	str_handler(const char *s)
{
	if (!s)
		return (ft_putstr_fd("(null)", 1));
	else
		return (ft_putstr_fd(s, 1));
}
