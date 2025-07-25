/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiennguy <hiennguy@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 13:55:50 by hiennguy          #+#    #+#             */
/*   Updated: 2025/07/25 13:52:50 by hiennguy         ###   ########.fr       */
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
		if (ft_isspace(*line))
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
		ft_free_2d(tokens);
		return(print_error(ERROR_MALLOC, NULL));
	}

	for (int i = 0; tokens[i] != NULL; ++i)
		ft_dprintf(1, "DEBUG!: tokens[%i]: <%s>\n", i, tokens[i]);
	if (ft_strcmp(tokens[0], "A") == 0)
	{
		if (parse_ambient(tokens, &minirt->scene) == FAIL)
			status = FAIL;
	}
	else if (ft_strcmp(tokens[0], "C") == 0)
	{
		if (parse_camera(tokens, &minirt->scene) == FAIL)
			status = FAIL;
	}
	else if (ft_strcmp(tokens[0], "L") == 0)
	{
		if (parse_light(tokens, &minirt->scene) == FAIL)
			status = FAIL;
	}
	else if (ft_strcmp(tokens[0], "sp") == 0)
	{
		if (parse_sphere(tokens, &minirt->scene) == FAIL)
		{
			status = FAIL;
			delete_minirt(minirt);
		}
	}
	else if (ft_strcmp(tokens[0], "pl") == 0)
	{
		if (parse_plane(tokens, &minirt->scene) == FAIL)
		{
			status = FAIL;
			delete_minirt(minirt);
		}
	}
	else if (ft_strcmp(tokens[0], "cy") == 0)
	{
		if (parse_cyl(tokens, &minirt->scene) == FAIL)
		{
			status = FAIL;
			delete_minirt(minirt);
		}
	}
    else
    {
		ft_dprintf(2, UNK_ELEMENT" %s\n", tokens[0]);
        status = FAIL;
		delete_minirt(minirt);
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

