/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiennguy <hiennguy@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/03 13:41:01 by hiennguy          #+#    #+#             */
/*   Updated: 2024/11/12 19:36:14 by hiennguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*substr;
	size_t	s_len;
	size_t	substr_length;

	s_len = ft_strlen(s);
	if (start >= s_len)
		return (ft_strdup(""));
	substr_length = len;
	if (len > s_len - start)
		substr_length = s_len - start;
	substr = malloc((substr_length + 1) * sizeof(char));
	if (substr == NULL)
		return (NULL);
	ft_strlcpy (substr, s + start, (substr_length + 1));
	return (substr);
}
