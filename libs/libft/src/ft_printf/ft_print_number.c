/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_number.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiennguy <hiennguy@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 15:22:27 by hiennguy          #+#    #+#             */
/*   Updated: 2024/11/20 15:35:17 by hiennguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/ft_printf.h"

static int	print_positive_number(long positive_number)
{
	int	count;
	int	temp;

	count = 0;
	if (positive_number > 9)
	{
		temp = print_positive_number(positive_number / 10);
		if (temp == -1)
			return (-1);
		count += temp;
	}
	if (ft_print_char((positive_number % 10) + '0') == -1)
		return (-1);
	count++;
	return (count);
}

int	ft_print_number(long nb)
{
	int	count;
	int	temp;

	count = 0;
	if (nb == -2147483648)
		return (write(1, "-2147483648", 11));
	if (nb < 0)
	{
		if (ft_print_char('-') == -1)
			return (-1);
		nb = -nb;
		count++;
	}
	temp = print_positive_number(nb);
	if (temp == -1)
		return (-1);
	count += temp;
	return (count);
}
