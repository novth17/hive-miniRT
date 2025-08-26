#include "mini_rt.h"

int	parse_ambient(char **tokens, t_scene *scene)
{
	if (check_id_args_count(tokens, "Ambient", 3) == FAIL)
		return (FAIL);
	scene->ambient.ratio = parse_float(tokens[1], &scene->is_valid);
	if (!scene->is_valid)
		return (print_error("Ambient: ratio: " ERROR_FLOAT, tokens[1]));
	if (scene->ambient.ratio < 0.0 || scene->ambient.ratio > 1.0)
		return (print_error("Ambient: ratio: " ERROR_IN_RANGE, tokens[1]));
	scene->ambient.color = parse_color(tokens[2], &scene->is_valid);
	if (!scene->is_valid)
		return (print_error("Ambient: " ERROR_COLOR, tokens[2]));
	scene->ambient.color = v3_mul_f32(scene->ambient.color, scene->ambient.ratio);
	return (SUCCESS);
}

int	parse_camera(char **tokens, t_scene *scene)
{
	if (check_id_args_count(tokens, "Camera", 4) == FAIL)
		return (FAIL);
	scene->camera.lookfrom = parse_vec3(tokens[1], &scene->is_valid);
	if (!scene->is_valid)
		return (print_error("Camera: origin: " ERROR_COORD, tokens[1]));
	scene->camera.lookat = parse_vec3(tokens[2], &scene->is_valid);
	if (!scene->is_valid)
		return (print_error("Camera: direction: " ERROR_COORD, tokens[2]));
	if (!is_in_range_vec3(scene->camera.lookat))
		return (print_error("Camera: ratio: " ERROR_IN_RANGE, NULL));
	if (!is_normalized(&scene->camera.lookat))
		return (print_error("Camera: ratio: " ERROR_NORM, NULL));
	scene->camera.fov = parse_float(tokens[3], &scene->is_valid);
	if (!scene->is_valid)
		return (print_error("Camera: FOV: " ERROR_FLOAT, tokens[3]));
	if (scene->camera.fov < 0 || scene->camera.fov > 180)
		return (print_error("Camera: " ERROR_FOV, tokens[3]));
	return (SUCCESS);
}

int	parse_light(char **tokens, t_scene *scene)
{
	t_object	*object;

	if (check_obj_args_count(tokens, "Light", 3, 4) == FAIL)
		return (FAIL);
	object = create_objects(scene, LIGHT);
	if (!object)
		return (FAIL);
	object->light.color = (t_color){{1.0, 1.0, 1.0}};
	object->light.origin = parse_vec3(tokens[1], &scene->is_valid);
	if (!scene->is_valid)
		return (print_error("Light: " ERROR_COORD, tokens[1]));
	object->light.bright_ratio = parse_float(tokens[2], &scene->is_valid);
	if (!scene->is_valid)
		return (print_error("Light: ratio: " ERROR_FLOAT, tokens[2]));
	if (object->light.bright_ratio < 0.0 || object->light.bright_ratio > 1.0)
		return (print_error("Light: ratio: " ERROR_IN_RANGE, tokens[2]));
	if (tokens[3])
	{
		object->light.color = parse_color(tokens[3], &scene->is_valid);
		if (!scene->is_valid)
			return (print_error("Light: " ERROR_COLOR, tokens[3]));
	}
	scene->lights_count++;
	object->light.color = v3_mul_f32(object->light.color, object->light.bright_ratio);
	return (SUCCESS);
}
