/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiennguy <hiennguy@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 17:11:38 by hiennguy          #+#    #+#             */
/*   Updated: 2024/11/03 12:12:48 by hiennguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/libft.h"

char	*ft_strrchr(const char *s, int c)
{
	const char		*last_occur;
	unsigned char	target;

	last_occur = NULL;
	target = (unsigned char)c;
	while (*s != '\0')
	{
		if (*s == target)
			last_occur = s;
		s++;
	}
	if (target == '\0')
		return ((char *)s);
	return ((char *)last_occur);
}
