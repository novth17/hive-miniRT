/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiennguy <hiennguy@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 20:44:56 by hiennguy          #+#    #+#             */
/*   Updated: 2025/07/21 16:17:48 by hiennguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/get_next_line.h"

static char	*create_line_from_cache(char *cache, int *newline_index)
{
	size_t	len;
	char	*line;

	len = 0;
	while (cache[len] && cache[len] != '\n')
		len++;
	len++;
	line = malloc(sizeof(char) * (len + 1));
	if (!line)
		return (NULL);
	ft_memcpy(line, cache, len);
	line[len] = '\0';
	if (len > 0 && line[len - 1] == '\n')
		*newline_index = len - 1;
	return (line);
}

static size_t	find_newline(char *line)
{
	char	*ptr;

	if (!line)
		return (0);
	ptr = line;
	while (*ptr != '\0')
	{
		if (*ptr == '\n')
			return (ptr - line + 1);
		ptr++;
	}
	return (ptr - line);
}

static int	check_read_status(ssize_t read_bytes, char *line, char *cache,
		char *buffer)
{
	if (read_bytes == -1)
	{
		free(line);
		free(buffer);
		ft_bzero(cache, BUFFER_SIZE + 1);
		return (-1);
	}
	if (read_bytes == 0)
	{
		ft_bzero(cache, BUFFER_SIZE + 1);
		return (0);
	}
	return (1);
}

static char	*read_and_extract_line(char *line, char *cache, int *newline_index,
		int fd)
{
	char	*buffer;
	ssize_t	read_bytes;
	size_t	line_size;
	int		status;

	buffer = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buffer)
		return (NULL);
	while (*newline_index == -1)
	{
		ft_bzero(buffer, (BUFFER_SIZE + 1));
		read_bytes = read(fd, buffer, BUFFER_SIZE);
		status = check_read_status(read_bytes, line, cache, buffer);
		if (status == -1)
			return (NULL);
		if (status == 0)
			break ;
		line_size = find_newline(buffer);
		ft_strlcpy_gnl(cache, &buffer[line_size], (BUFFER_SIZE + 1));
		buffer[line_size] = '\0';
		line = ft_strjoin_gnl(line, buffer, newline_index);
	}
	free(buffer);
	return (line);
}

char	*get_next_line(int fd)
{
	static char	cache[BUFFER_SIZE + 1];
	char		*line;
	int			newline_index;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	newline_index = -1;
	line = create_line_from_cache(cache, &newline_index);
	if (!line)
		return (NULL);
	ft_strlcpy_gnl(cache, &cache[newline_index + 1], BUFFER_SIZE + 1);
	line = read_and_extract_line(line, cache, &newline_index, fd);
	if (!line || line[0] == '\0')
	{
		free(line);
		return (NULL);
	}
	return (line);
}
