/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_hex_lower.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiennguy <hiennguy@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 15:22:27 by hiennguy          #+#    #+#             */
/*   Updated: 2024/11/20 15:34:23 by hiennguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/ft_printf.h"

int	ft_print_hex_lower(unsigned long nb)
{
	const char	*hex_char = "0123456789abcdef";
	int			count;

	count = 0;
	if (nb >= 16)
		count += ft_print_hex_lower (nb / 16);
	count += write(1, &hex_char[nb % 16], 1);
	return (count);
}
