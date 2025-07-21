/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiennguy <hiennguy@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 13:55:50 by hiennguy          #+#    #+#             */
/*   Updated: 2025/07/21 21:08:14 by hiennguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "mini_rt.h"

static int parse_file(t_minirt *minirt, char *filename);
static int parse_line(char *line, t_minirt *minirt);

int	parse(t_minirt *minirt, char **filename)
{
	if (parse_file(minirt, *filename) == FAIL)
		return (FAIL);

	return (SUCCESS);
}


static int parse_file(t_minirt *minirt, char *filename)
{
	int		fd;
	char	*line = NULL;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
		exit_error("Failed to open scene file");

	while ((line = get_next_line(fd)))
	{
		if (line[0] == '\n' || line[0] == '\0')
		{
			free(line);
			continue;
		}
		if (parse_line(line, minirt) == FAIL)
		 	exit_error("Scene parsing failed");
		free(line);
	}
	close(fd);
	return (SUCCESS);
}

static int parse_line(char *line, t_minirt *minirt)
{

}





