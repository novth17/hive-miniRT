/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiennguy <hiennguy@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 17:11:38 by hiennguy          #+#    #+#             */
/*   Updated: 2024/11/06 14:03:44 by hiennguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/libft.h"

static int	ft_is_in_set(char c, const char *set)
{
	while (*set != '\0')
	{
		if (c == *set)
		{
			return (1);
		}
		set++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	start;
	size_t	end;
	char	*trimmed_str;

	if (*s1 == '\0')
		return (ft_strdup(""));
	if (set == NULL || *set == '\0')
		return (ft_strdup(s1));
	start = 0;
	end = ft_strlen(s1) - 1;
	while (s1[start] && ft_is_in_set(s1[start], set))
		start++;
	if (s1[start] == '\0')
		return (ft_strdup(""));
	while (end != start && ft_is_in_set(s1[end], set))
		end--;
	trimmed_str = malloc((end - start + 2) * sizeof(char));
	if (trimmed_str == NULL)
		return (NULL);
	ft_strlcpy(trimmed_str, s1 + start, end - start + 2);
	return (trimmed_str);
}
