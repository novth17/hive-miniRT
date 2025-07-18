/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_hex_pointer.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiennguy <hiennguy@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 15:22:27 by hiennguy          #+#    #+#             */
/*   Updated: 2025/07/18 18:08:34 by hiennguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/ft_dprintf.h"

int	ft_print_hex_pointer(int fd, void *ptr)
{
	unsigned long	nb;

	if (!ptr)
		return (write(fd, "(nil)", 5));
	nb = (unsigned long)(ptr);
	write(fd, "0x", 2);
	return (ft_print_hex_lower(fd, nb) + 2);
}
