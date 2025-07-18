/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_number.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiennguy <hiennguy@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 15:22:27 by hiennguy          #+#    #+#             */
/*   Updated: 2025/07/18 18:08:44 by hiennguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/ft_dprintf.h"

static int	print_positive_number(int fd, long positive_number)
{
	int	count;
	int	temp;

	count = 0;
	if (positive_number > 9)
	{
		temp = print_positive_number(fd, positive_number / 10);
		if (temp == -1)
			return (-1);
		count += temp;
	}
	if (ft_print_char(fd, (positive_number % 10) + '0') == -1)
		return (-1);
	count++;
	return (count);
}

int	ft_print_number(int fd, long nb)
{
	int	count;
	int	temp;

	count = 0;
	if (nb == -2147483648)
		return (write(fd, "-2147483648", 11));
	if (nb < 0)
	{
		if (ft_print_char(fd, '-') == -1)
			return (-1);
		nb = -nb;
		count++;
	}
	temp = print_positive_number(fd, nb);
	if (temp == -1)
		return (-1);
	count += temp;
	return (count);
}
