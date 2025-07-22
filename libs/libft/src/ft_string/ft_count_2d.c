/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_count_2d.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiennguy <hiennguy@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 17:06:55 by hiennguy          #+#    #+#             */
/*   Updated: 2025/07/22 17:08:01 by hiennguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/libft.h"

int	ft_count_2d(char **tokens)
{
	int	count;

	count = 0;
	if (!tokens)
		return (0);
	while (tokens[count])
		count++;
	return (count);
}
