/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_is_only_whitespace.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiennguy <hiennguy@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 14:50:00 by hiennguy          #+#    #+#             */
/*   Updated: 2025/07/26 14:52:09 by hiennguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/libft.h"

bool	is_only_whitespace(const char *s)
{
	while (*s)
	{
		if (!ft_isspace(*s))
			return (false);
		s++;
	}
	return (true);
}
