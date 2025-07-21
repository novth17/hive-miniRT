/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiennguy <hiennguy@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 20:44:49 by hiennguy          #+#    #+#             */
/*   Updated: 2025/07/21 17:28:57 by hiennguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/get_next_line.h"

char	*ft_strjoin_gnl(char *s1, char *s2, int *newline_index)
{
	char	*combined_str;
	size_t	s1_len;
	size_t	s2_len;
	size_t	total_len;

	if (!s1 || !s2)
		return (NULL);
	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	total_len = s1_len + s2_len;
	combined_str = (char *)malloc(sizeof(char) * (total_len + 1));
	if (!combined_str)
	{
		free(s1);
		return (NULL);
	}
	ft_memcpy(combined_str, s1, s1_len);
	free(s1);
	ft_memcpy(combined_str + s1_len, s2, s2_len + 1);
	if (total_len > 0 && *(combined_str + total_len - 1) == '\n')
		*newline_index = 0;
	return (combined_str);
}

void	ft_strlcpy_gnl(char *dst, const char *src, size_t dst_size)
{
	size_t	i;

	i = 0;
	if (dst_size > 0)
	{
		while (src[i] && i < dst_size - 1)
		{
			dst[i] = src[i];
			i++;
		}
		dst[i] = '\0';
	}
}

void	ft_bzero_gnl(void *s, size_t n)
{
	size_t			i;
	unsigned char	*ptr;

	ptr = (unsigned char *)s;
	i = 0;
	while (i < n)
	{
		ptr[i] = 0;
		i++;
	}
}

void	*ft_memcpy_gnl(void *dst, const void *src, size_t n)
{
	size_t		i;
	char		*dest;
	const char	*source;

	if (!dst || !src)
		return (NULL);
	dest = (char *)dst;
	source = (const char *)src;
	i = 0;
	while (i < n)
	{
		dest[i] = source[i];
		i++;
	}
	return (dst);
}
