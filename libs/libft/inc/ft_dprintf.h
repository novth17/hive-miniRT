/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dprintf.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiennguy <hiennguy@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 15:22:27 by hiennguy          #+#    #+#             */
/*   Updated: 2025/07/18 18:08:58 by hiennguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_DPRINTF_H
# define FT_DPRINTF_H

# include "libft.h"

int		ft_print_char(int fd, int c);
int		ft_print_hex_lower(int fd, unsigned long nb);
int		ft_print_hex_pointer(int fd, void *ptr);
int		ft_print_hex_upper(int fd, unsigned long nb);
int		ft_print_number(int fd, long nb);
int		ft_print_string(int fd, char *str);
int		ft_dprintf(int fd, const char *format_string, ...);

#endif
