/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiennguy <hiennguy@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 15:22:27 by hiennguy          #+#    #+#             */
/*   Updated: 2025/03/01 14:53:55 by hiennguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include "libft.h"

int		ft_print_char(int c);
int		ft_print_hex_lower(unsigned long nb);
int		ft_print_hex_pointer(void *ptr);
int		ft_print_hex_upper(unsigned long nb);
int		ft_print_number(long nb);
int		ft_print_string(char *str);
int		ft_printf(const char *format_string, ...);

#endif
