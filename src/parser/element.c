/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   element.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiennguy <hiennguy@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 16:30:30 by hiennguy          #+#    #+#             */
/*   Updated: 2025/07/23 21:16:16 by hiennguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

int parse_ambient(char **tokens, t_scene *scene)
{
	scene->is_valid = true;

	if (ft_count_2d(tokens) != 3)
	{
		ft_dprintf(2, "Ambient: Number of Ambient arguments must be 3!\n");
		return (FAIL);
	}
	scene->ambient.ratio = parse_float(tokens[1], &scene->is_valid);
	if (!scene->is_valid)
		return (FAIL);

	if (scene->ambient.ratio < 0.0 || scene->ambient.ratio > 1.0)
	{
		ft_dprintf(2, "Ambient: Lighting ratio must be in range [0.0,1.0]!\n");
		return (FAIL);
	}
	scene->ambient.color = parse_color(tokens[2], &scene->is_valid);
	if (!scene->is_valid)
	{
		ft_dprintf(2, "Ambient: Invalid color format! ");
		ft_dprintf(2, "Must have correct commas and in range [0,255]\n");
		return (FAIL);
	}
	return (SUCCESS);
}

int parse_camera(char **tokens, t_scene *scene)
{
	scene->is_valid = true;

	if (ft_count_2d(tokens) != 4)
	{
		ft_dprintf(2, "Camera: Number of Camera arguments must be 4!\n");
		return (FAIL);
	}
	scene->camera.origin = parse_vec3(tokens[1], &scene->is_valid);
	if (!scene->is_valid)
	{
		ft_dprintf(2, "Camera: Invalid origin vector format: %s\n", tokens[1]);
		return (FAIL);
	}
	scene->camera.direction = parse_vec3(tokens[2], &scene->is_valid);
	if (!scene->is_valid)
	{
		ft_dprintf(2, "Camera: Invalid origin vector format: %s\n", tokens[2]);
		return (FAIL);
	}
	if (!is_normalized_vec3(scene->camera.direction))
	{
		ft_dprintf(2, "Camera: Direction vector must be in range [-1,1]!\n");
		return (FAIL);
	}
	scene->camera.fov = parse_float(tokens[3], &scene->is_valid);
	if (scene->camera.fov < 0 || scene->camera.fov > 180)
	{
		ft_dprintf(2, "Camera: FOV must be in range [0,180]: %s\n", tokens[3]);
		return (FAIL);
	}
	return (SUCCESS);
}

