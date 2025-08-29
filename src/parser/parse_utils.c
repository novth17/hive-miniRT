/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiennguy <hiennguy@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 17:21:27 by hiennguy          #+#    #+#             */
/*   Updated: 2025/08/28 22:53:02 by hiennguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

char	*set_next_line(char **line, int fd)
{
	*line = get_next_line(fd);
	return (*line);
}

void	check_fd(t_minirt *minirt, int fd)
{
	if (fd < 0)
	{
		ft_dprintf(2, RED"Error\n"ERROR_NOT_EXIST"\n\n"RESET);
		print_guide();
		delete_minirt(minirt);
		exit(FAIL);
	}
}

void	normalize_whitespace(char *line)
{
	while (*line)
	{
		if (ft_isspace(*line))
			*line = ' ';
		line++;
	}
}
