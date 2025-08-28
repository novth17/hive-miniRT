/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 19:13:42 by ltaalas           #+#    #+#             */
/*   Updated: 2025/08/28 19:13:42 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

static
uint8_t	num_length(uint32_t num)
{
	uint8_t	i;

	i = 0;
	if (num == 0)
		return (1);
	while (num > 0)
	{
		num = num / 10;
		i++;
	}
	return (i);
}

// concatinate a number to the end of a t_string
bool	cat_uint_to_str(t_string *dest, size_t num)
{
	const uint8_t	num_len = num_length(num);
	uint32_t		i;

	if (dest->len + num_len + 1 > dest->size)
		return (FAIL);
	dest->buf[dest->len + num_len] = '\0';
	i = num_len;
	while (i--)
	{
		dest->buf[dest->len + i] = (num % 10) + '0';
		num = num / 10;
	}
	dest->len += num_len;
	return (SUCCESS);
}

// concatinate a c string to the end of a t_string
bool	cat_cstring_to_string(t_string *dest, const char *cstr)
{
	size_t	cstr_len;

	if (cstr == NULL)
		return (1);
	cstr_len = ft_strlen(cstr);
	if (dest->size <= dest->len + cstr_len)
		return (1);
	ft_memmove(&dest->buf[dest->len], cstr, cstr_len);
	dest->len += cstr_len;
	dest->buf[dest->len] = '\0';
	return (0);
}

// concatinate up to n characters of a c string to the end of a t_string
bool	cat_cstring_to_string_n(t_string *dest, const char *cstr, uint32_t n)
{
	size_t	cstr_len;
	size_t	to_write;

	if (cstr == NULL)
		return (1);
	cstr_len = ft_strlen(cstr);
	to_write = n;
	if (cstr_len < n)
		to_write = n;
	if (dest->size <= dest->len + to_write)
		return (1);
	ft_memmove(&dest->buf[dest->len], cstr, to_write);
	dest->len += to_write;
	dest->buf[dest->len] = '\0';
	return (0);
}
