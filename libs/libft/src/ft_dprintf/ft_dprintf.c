/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dprintf.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiennguy <hiennguy@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 15:22:27 by hiennguy          #+#    #+#             */
/*   Updated: 2025/07/18 18:09:48 by hiennguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/ft_dprintf.h"
#include <stdarg.h>

static int	handle_format(int fd, char c, va_list *args)
{
	if (c == 'c')
		return (ft_print_char(fd, va_arg(*args, int)));
	if (c == 's')
		return (ft_print_string(fd, va_arg(*args, char *)));
	if (c == 'p')
		return (ft_print_hex_pointer(fd, va_arg(*args, void *)));
	if (c == 'x')
		return (ft_print_hex_lower(fd, va_arg(*args, unsigned int)));
	if (c == 'X')
		return (ft_print_hex_upper(fd, va_arg(*args, unsigned int)));
	if (c == 'd' || c == 'i')
		return (ft_print_number(fd, va_arg(*args, int)));
	if (c == 'u')
		return (ft_print_number(fd, va_arg(*args, unsigned int)));
	if (c == '%')
		return (ft_print_char(fd, '%'));
	return (ft_print_char(fd, c));
}

int	ft_dprintf(int fd, const char *format_string, ...)
{
	int		i;
	int		printed_chars;
	va_list	args;

	va_start(args, format_string);
	i = 0;
	printed_chars = 0;
	while (format_string[i] != '\0')
	{
		if (format_string[i] == '%' && format_string[i + 1] != '\0')
		{
			printed_chars += handle_format(fd, format_string[i + 1], &args);
			i++;
		}
		else
		{
			write(fd, &format_string[i], 1);
			printed_chars++;
		}
		i++;
	}
	return (printed_chars);
}
