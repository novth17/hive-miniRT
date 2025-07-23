/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   element.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiennguy <hiennguy@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 16:30:30 by hiennguy          #+#    #+#             */
/*   Updated: 2025/07/23 22:15:34 by hiennguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

int parse_ambient(char **tokens, t_scene *scene)
{
	scene->is_valid = true;
	if (check_id_args_count(tokens, "Ambient", 3) == FAIL)
		return (FAIL);

	scene->ambient.ratio = parse_float(tokens[1], &scene->is_valid);
	if (!scene->is_valid)
		return (print_error("Ambient: Invalid lighting ratio", tokens[1]));

	if (scene->ambient.ratio < 0.0 || scene->ambient.ratio > 1.0)
		return (print_error("Ambient: Lighting ratio must be in range [0.0,1.0]", tokens[1]));

	scene->ambient.color = parse_color(tokens[2], &scene->is_valid);
	if (!scene->is_valid)
		return (print_error("Ambient: Invalid color format (commas and [0,255])", tokens[2]));

	return (SUCCESS);
}

int parse_camera(char **tokens, t_scene *scene)
{
	scene->is_valid = true;
	if (check_id_args_count(tokens, "Camera", 4) == FAIL)
		return (FAIL);

	scene->camera.origin = parse_vec3(tokens[1], &scene->is_valid);
	if (!scene->is_valid)
		return (print_error("Camera: Wrong origin v3 format", tokens[1]));

	scene->camera.direction = parse_vec3(tokens[2], &scene->is_valid);
	if (!scene->is_valid)
		return (print_error("Camera: Wrong direction v3 format", tokens[2]));

	if (!is_normalized_vec3(scene->camera.direction))
		return (print_error("Camera: Direction vector must be in range [-1,1]", NULL));

	scene->camera.fov = parse_float(tokens[3], &scene->is_valid);
	if (!scene->is_valid)
		return (print_error("Camera: FOV must be a float", tokens[3]));
	if (scene->camera.fov < 0 || scene->camera.fov > 180)
		return (print_error("Camera: FOV must be in range [0,180]", tokens[3]));

	return (SUCCESS);
}

int parse_light(char **tokens, t_scene *scene)
{
	scene->is_valid = true;
	if (check_id_args_count(tokens, "Light", 4) == FAIL)
		return (FAIL);

	scene->light.origin = parse_vec3(tokens[1], &scene->is_valid);
	if (!scene->is_valid)
		return print_error("Light: Wrong origin v3 format", tokens[1]);

	scene->light.bright_ratio = parse_float(tokens[2], &scene->is_valid);
	if (!scene->is_valid)
		return print_error("Light: Invalid lighting ratio", tokens[2]);

	if (scene->light.bright_ratio < 0.0 || scene->light.bright_ratio > 1.0)
		return print_error("Light: Lighting ratio must be in range [0.0,1.0]", tokens[2]);

	scene->light.color = parse_color(tokens[3], &scene->is_valid);
	if (!scene->is_valid)
		return print_error("Light: Invalid color format (commas and [0,255])", tokens[3]);

	return (SUCCESS);
}
