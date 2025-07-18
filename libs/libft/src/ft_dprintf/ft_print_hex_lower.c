/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_hex_lower.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiennguy <hiennguy@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 15:22:27 by hiennguy          #+#    #+#             */
/*   Updated: 2025/07/18 18:08:31 by hiennguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/ft_dprintf.h"

int	ft_print_hex_lower(int fd, unsigned long nb)
{
	const char	*hex_char = "0123456789abcdef";
	int			count;

	count = 0;
	if (nb >= 16)
		count += ft_print_hex_lower (fd, nb / 16);
	count += write(fd, &hex_char[nb % 16], 1);
	return (count);
}
