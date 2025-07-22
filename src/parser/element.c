/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   element.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiennguy <hiennguy@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 16:30:30 by hiennguy          #+#    #+#             */
/*   Updated: 2025/07/22 21:49:53 by hiennguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

// typedef struct s_color
// {
// 	int r;
// 	int g;
// 	int b;
// }				t_color;

t_color parse_color(char *str)
{
	// must do it without split
	// make sure they re int
	// between 0-255

	// while (color_arr)
	// {
	// 	if (ft_isdigit(*color_arr))
	// 		*color_arr++;
	// }
}
int parse_ambient(char **tokens, t_scene *scene)
{
	if (ft_count_2d(tokens) != 3)
	{
		ft_dprintf(2, "Ambient: Number of arguments must be 3!\n");
		return (FAIL);
	}
	bool is_valid = true;
	scene->ambient.ratio = parse_float(tokens[1], &is_valid);
	if (!is_valid)
		return (FAIL);

	if (scene->ambient.ratio < 0.0 || scene->ambient.ratio > 1.0)
	{
		ft_dprintf(2, "Ambient: Lighting ratio must be in range [0.0,1.0]!\n");
		return (FAIL);
	}
	scene->ambient.color = parse_color(tokens[2]); // implement or reuse
	return (SUCCESS);
}

