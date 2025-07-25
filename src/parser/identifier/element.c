/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   element.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiennguy <hiennguy@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 16:30:30 by hiennguy          #+#    #+#             */
/*   Updated: 2025/07/25 20:07:30 by hiennguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

int parse_ambient(char **tokens, t_scene *scene)
{
	if (check_id_args_count(tokens, "Ambient", 3) == FAIL)
		return (FAIL);

	scene->ambient.ratio = parse_float(tokens[1], &scene->is_valid);
	if (!scene->is_valid)
		return (print_error("Ambient: ratio: "ERROR_FLOAT, tokens[1]));
	if (scene->ambient.ratio < 0.0 || scene->ambient.ratio > 1.0)
		return (print_error("Ambient: ratio: "ERROR_NORM, tokens[1]));
	scene->ambient.color = parse_color(tokens[2], &scene->is_valid);
	if (!scene->is_valid)
		return (print_error("Ambient: "ERROR_COLOR, tokens[2]));
	return (SUCCESS);
}

int parse_camera(char **tokens, t_scene *scene)
{
	if (check_id_args_count(tokens, "Camera", 4) == FAIL)
		return (FAIL);

	scene->camera.origin = parse_vec3(tokens[1], &scene->is_valid);
	if (!scene->is_valid)
		return (print_error("Camera: origin: "ERROR_COORD, tokens[1]));

	scene->camera.direction = parse_vec3(tokens[2], &scene->is_valid);
	if (!scene->is_valid)
		return (print_error("Camera: direction: "ERROR_COORD, tokens[2]));

	if (!is_normalized_vec3(scene->camera.direction))
		return (print_error("Camera: ratio: "ERROR_NORM, NULL));

	scene->camera.fov = parse_float(tokens[3], &scene->is_valid);
	if (!scene->is_valid)
		return (print_error("Camera: FOV: "ERROR_FLOAT, tokens[3]));
	if (scene->camera.fov < 0 || scene->camera.fov > 180)
		return (print_error("Camera: FOV must be in range [0,180]", tokens[3]));

	return (SUCCESS);
}

int parse_light(char **tokens, t_scene *scene)
{
	if (check_id_args_count(tokens, "Light", 4) == FAIL)
		return (FAIL);

	scene->light.origin = parse_vec3(tokens[1], &scene->is_valid);
	if (!scene->is_valid)
		return print_error("Light: "ERROR_COORD, tokens[1]);

	scene->light.bright_ratio = parse_float(tokens[2], &scene->is_valid);
	if (!scene->is_valid)
		return print_error("Light: ratio: "ERROR_FLOAT, tokens[2]);

	if (scene->light.bright_ratio < 0.0 || scene->light.bright_ratio > 1.0)
		return print_error("Light: ratio: "ERROR_NORM, tokens[2]);

	scene->light.color = parse_color(tokens[3], &scene->is_valid);
	if (!scene->is_valid)
		return print_error("Light: "ERROR_COLOR, tokens[3]);

	return (SUCCESS);
}
