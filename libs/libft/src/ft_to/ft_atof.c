/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atof.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiennguy <hiennguy@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 19:51:22 by hiennguy          #+#    #+#             */
/*   Updated: 2025/07/22 17:32:23 by hiennguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/libft.h"

double	ft_atof(const char *str)
{
	double	result;
	int		sign;
	double	decimal_count;

	result = 0.0;
	sign = 1;
	decimal_count = 1;
	if (str == NULL)
		return (0.0);
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '-')
		sign = -1;
	if (*str == '-' || *str == '+')
		str++;
	while (*str >= '0' && *str <= '9')
		result = result * 10 + (*str++ - '0');
	if (*str == '.')
		str++;
	while (*str >= '0' && *str <= '9')
	{
		result = result * 10 + (*str++ - '0');
		decimal_count *= 10;
	}
	return ((result / decimal_count) * sign);
}
