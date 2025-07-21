/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiennguy <hiennguy@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 20:45:39 by hiennguy          #+#    #+#             */
/*   Updated: 2025/07/21 17:28:15 by hiennguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1024
# endif

# include "libft.h"

char	*ft_strjoin_gnl(char *s1, char *s2, int *newline_index);
void	ft_strlcpy_gnl(char *dst, const char *src, size_t dst_size);
void	*ft_memcpy_gnl(void *dst, void const *src, size_t n);
void	ft_bzero_gnl(void *s, size_t n);
char	*get_next_line(int fd);

#endif
