/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiennguy <hiennguy@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 17:11:38 by hiennguy          #+#    #+#             */
/*   Updated: 2024/11/01 15:47:49 by hiennguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char		*d;
	const unsigned char	*s;

	d = (unsigned char *)dest;
	s = (const unsigned char *)src;
	if (d <= s)
	{
		ft_memcpy(dest, src, n);
	}
	else
	{
		while (n > 0)
		{
			d[n - 1] = s[n - 1];
			n--;
		}
	}
	return (dest);
}
