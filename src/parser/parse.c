/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiennguy <hiennguy@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 13:55:50 by hiennguy          #+#    #+#             */
/*   Updated: 2025/07/22 20:45:37 by hiennguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "mini_rt.h"

//valgrind --leak-check=full  --suppressions=mlx42.supp ./mini_rt test_wrong.rt

static int	parse_line(char *line, t_minirt *minirt);

int	parse_file(t_minirt *minirt, char *filename)
{
	int		fd;
	char	*line = NULL;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
		exit_error("No such file exists!");
	while ((line = get_next_line(fd)))
	{
		if (line[0] != '\n' && line[0] != '\0' && parse_line(line, minirt) == FAIL)
		{
			free(line);
			exit_error("Scene parsing failed");
		}
		free(line);
	}
	close(fd);
	return (SUCCESS);
}

static void normalize_whitespace(char *line)
{
	while (*line)
	{
		if (*line == '\t' || *line == '\v' || *line == '\f' || *line == '\r')
			*line = ' ';
		line++;
	}
}

static int	parse_line(char *line, t_minirt *minirt)
{
	char **tokens;
	int status;

	status = SUCCESS;
	ft_dprintf(1, "\nDEBUG!: line: %s", line);
	normalize_whitespace(line);
	tokens = ft_split(line, ' ');

	if (!tokens || !tokens[0])
	{
		ft_dprintf(2, "malloc failed!");
		ft_free_2d(tokens);
		return(FAIL);
	}
	ft_dprintf(1, "DEBUG!: tokens[0]: %s\n", tokens[0]);
	if (ft_strcmp(tokens[0], "A") == 0)
	{
		if (parse_ambient(tokens, &minirt->scene) == FAIL)
			status = FAIL;
	}
	else if (ft_strcmp(tokens[0], "C") == 0)
		ft_dprintf(2, "camera!\n");
	else if (ft_strcmp(tokens[0], "L") == 0)
		ft_dprintf(2, "light!\n");
	else if (ft_strcmp(tokens[0], "sp") == 0)
		ft_dprintf(2, "sphere!\n");
	else if (ft_strcmp(tokens[0], "pl") == 0)
		ft_dprintf(2, "PLANE!\n");
	else if (ft_strcmp(tokens[0], "cy") == 0)
		ft_dprintf(2, "CYLINDER!\n");
    else
    {
		ft_dprintf(2, "Error: Unknown element: [%s]\n", tokens[0]);
        status = FAIL;
    }
	ft_free_2d(tokens);
	return status;
}

// static int	parse_line(char *line, t_minirt *minirt)
// {
// 	char **tokens;

// 	tokens = ft_split(line, ' ');
// 	if (!tokens || !tokens[0])
// 	{
// 		ft_free_2d(tokens);
// 		return(FAIL);
// 	}
// 	if (ft_strcmp(tokens[0], "A") == 0)
// 		return (parse_ambient(tokens, minirt->scene));
// 	else if (ft_strcmp(tokens[0], "C") == 0)
// 		return (parse_camera(tokens, minirt->scene));
// 	else if (ft_strcmp(tokens[0], "L") == 0)
// 		return (parse_light(tokens, minirt->scene));
// 	else if (ft_strcmp(tokens[0], "sp") == 0)
// 		return (parse_sphere(tokens, minirt->scene));
// 	else if (ft_strcmp(tokens[0], "pl") == 0)
// 		return (parse_plane(tokens, minirt->scene));
// 	else if (ft_strcmp(tokens[0], "cy") == 0)
// 		return (parse_cylinder(tokens, minirt->scene));
//     else
//     {
//         free_split(tokens);
// 		ft_dprintf(2, "Unknown element!");
//         return (FAIL);
//     }
// }





