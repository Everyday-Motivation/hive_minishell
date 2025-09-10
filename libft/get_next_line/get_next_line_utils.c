/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timurray <timurray@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 12:45:05 by timurray          #+#    #+#             */
/*   Updated: 2025/08/19 13:45:37 by timurray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

size_t	ft_strlen(const char *s)
{
	size_t	count;

	count = 0;
	while (*(s + count))
		count++;
	return (count);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	s1_length;
	size_t	s2_length;
	size_t	s_size;
	char	*s;

	if (!s1 || !s2)
		return (NULL);
	s1_length = ft_strlen(s1);
	s2_length = ft_strlen(s2);
	s_size = s1_length + s2_length + 1;
	s = (char *)malloc((s_size) * sizeof(char));
	if (!s)
		return (NULL);
	ft_memcpy(s, s1, s1_length);
	ft_memcpy(s + s1_length, s2, s2_length);
	s[s_size - 1] = '\0';
	return (s);
}

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char		*temp_dest;
	const unsigned char	*temp_src;

	temp_dest = dest;
	temp_src = (const unsigned char *)src;
	while (n > 0)
	{
		*temp_dest++ = *temp_src++;
		n--;
	}
	return (dest);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	s_len;
	size_t	substr_len;
	char	*substr;

	if (!s)
		return (NULL);
	s_len = ft_strlen(s);
	if (start >= s_len)
		return (ft_strdup(""));
	substr_len = s_len - start;
	if (len > (substr_len))
		len = (substr_len);
	substr = (char *)malloc(len + 1);
	if (!substr)
		return (NULL);
	ft_memcpy(substr, s + start, len);
	substr[len] = '\0';
	return (substr);
}

char	*ft_strdup(const char *s)
{
	char	*strdup;
	size_t	len;

	len = ft_strlen(s);
	strdup = (char *)malloc((len + 1) * sizeof(char));
	if (strdup == NULL)
		return (NULL);
	ft_memcpy(strdup, s, len);
	strdup[len] = '\0';
	return (strdup);
}
