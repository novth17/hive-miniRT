/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_hex_upper.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiennguy <hiennguy@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 15:22:27 by hiennguy          #+#    #+#             */
/*   Updated: 2025/07/18 18:08:37 by hiennguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/ft_dprintf.h"

int	ft_print_hex_upper(int fd, unsigned long nb)
{
	const char	*hex_char = "0123456789ABCDEF";
	int			count;

	count = 0;
	if (nb >= 16)
		count += ft_print_hex_upper (fd, nb / 16);
	count += write(fd, &hex_char[nb % 16], 1);
	return (count);
}
