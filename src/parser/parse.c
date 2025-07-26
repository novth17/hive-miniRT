/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiennguy <hiennguy@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 13:55:50 by hiennguy          #+#    #+#             */
/*   Updated: 2025/07/26 16:12:44 by hiennguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "mini_rt.h"

//valgrind --leak-check=full  --suppressions=mlx42.supp ./mini_rt test_wrong.rt

static int	parse_line(char *line, t_minirt *minirt);
static void	normalize_whitespace(char *line);
static int	parse_elements(char **tokens, t_minirt *minirt);

int	parse_file(t_minirt *minirt, char *filename)
{
	int		fd;
	char	*line;

	line = NULL;
	fd = open(filename, O_RDONLY);
	if (fd < 0)
		exit_error(minirt, ERROR_NOT_EXIST);
	while ((line = get_next_line(fd)))
	{
		if (!is_only_whitespace(line))
		{
			minirt->file_has_content = true;
			if (parse_line(line, minirt) == FAIL)
			{
				delete_minirt(minirt);
				free(line);
				exit(FAIL);
			}
		}
		free(line);
	}
	close(fd);
	if (!minirt->file_has_content)
		exit_error(minirt, ERROR_EMPTY);
	return (SUCCESS);
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

	status = parse_elements(tokens, minirt);
	ft_free_2d(tokens);
	return (status);
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

static int	parse_elements(char **tokens, t_minirt *minirt)
{
	if (ft_strcmp(tokens[0], "A") == 0)
		return parse_ambient(tokens, &minirt->scene);
	else if (ft_strcmp(tokens[0], "C") == 0)
		return parse_camera(tokens, &minirt->scene);
	else if (ft_strcmp(tokens[0], "L") == 0)
		return parse_light(tokens, &minirt->scene);
	else if (ft_strcmp(tokens[0], "sp") == 0)
		return parse_sphere(tokens, &minirt->scene);
	else if (ft_strcmp(tokens[0], "pl") == 0)
		return parse_plane(tokens, &minirt->scene);
	else if (ft_strcmp(tokens[0], "cy") == 0)
		return parse_cyl(tokens, &minirt->scene);
	print_error(UNK_ELEMENT, tokens[0]);
	return (FAIL);
}

