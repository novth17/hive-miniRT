/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiennguy <hiennguy@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 13:55:50 by hiennguy          #+#    #+#             */
/*   Updated: 2025/07/21 16:12:41 by hiennguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "mini_rt.h"
# include <fcntl.h>

static int	parse_file(t_minirt *minirt, char **argv)
{
	int		fd;
	char	*line;

	fd = open(argv[1], O_RDONLY);
	if (fd < 0)
		exit_error("Failed to open scene file");

	while ((line = get_next_line(fd)))
	{
		if (line[0] == '\n' || line[0] == '\0')
		{
			free(line);
			continue;
		}
		// if (parse_line(line, &minirt->scene) == FAIL)
		// {
		// 	free(line);
		// 	exit_error("Scene parsing failed");
		// }
		// free(line);
	}
	close(fd);
	return (SUCCESS);
}


int	parse(t_minirt *minirt, char **argv)
{
	parse_file(&minirt->scene, argv[1]);
	//parse_input;
	//parse_files;
		//parse_line();
}
