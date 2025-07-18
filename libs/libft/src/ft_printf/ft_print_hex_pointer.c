/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_hex_pointer.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiennguy <hiennguy@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 15:22:27 by hiennguy          #+#    #+#             */
/*   Updated: 2024/11/20 19:02:39 by hiennguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/ft_printf.h"

int	ft_print_hex_pointer(void *ptr)
{
	unsigned long	nb;

	if (!ptr)
		return (write(1, "(nil)", 5));
	nb = (unsigned long)(ptr);
	write(1, "0x", 2);
	return (ft_print_hex_lower(nb) + 2);
}
