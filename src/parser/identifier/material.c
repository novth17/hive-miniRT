#include "mini_rt.h"

// int parse_material(char **t, t_scene *scene, t_material *m, int o)
// {
// 	m->color = parse_color(t[o], &scene->is_valid);
// 	if (!scene->is_valid)
// 		return print_error("Material: color: " ERROR_COLOR, t[o]);
// 	m->diffuse = 0.0f;
// 	m->specular_probability = 0.0f;
// 	m->emitter = 0.0f;
// 	if (t[o + 1] && t[o + 2] && t[o + 3] && t[o + 4])
// 	{
// 		m->diffuse = parse_float(t[o + 1], &scene->is_valid);
// 		if (!scene->is_valid)
// 			return (print_error("Material: diffuse: " ERROR_FLOAT, t[o + 1]));
// 		m->specular_probability = parse_float(t[o + 2], &scene->is_valid);
// 		if (!scene->is_valid)
// 			return (print_error("Material: specular: " ERROR_FLOAT, t[o + 2]));
// 		m->emitter = parse_float(t[o + 3], &scene->is_valid);
// 		if (!scene->is_valid)
// 			return (print_error("Material: emitter: " ERROR_FLOAT, t[o + 3]));
// 		m->specular_color = parse_color(t[o + 4], &scene->is_valid);
// 		if (!scene->is_valid)
// 			return (print_error("Material: sp_color: " ERROR_COLOR, t[o + 4]));
// 	}
// 	return (SUCCESS);
// }

int parse_material(char **t, t_scene *scene, t_material *m, int o)
{
	const int count = ft_count_2d(t) - o;

	m->color = parse_color(t[o], &scene->is_valid);
	if (!scene->is_valid)
		return print_error("Material: color: " ERROR_COLOR, t[o]);
	m->diffuse = 0.0f;
	m->specular_probability = 0.0f;
	m->emitter = 0.0f;
	m->specular_color = (t_v3){{1.0, 1.0, 1.0}};
	if (count > 1)
	{
		m->diffuse = parse_float(t[o + 1], &scene->is_valid);
		if (!scene->is_valid)
			return (print_error("Material: diffuse: " ERROR_FLOAT, t[o + 1]));
	}
	if (count > 2)
	{
		m->specular_probability = parse_float(t[o + 2], &scene->is_valid);
		if (!scene->is_valid)
			return (print_error("Material: specular: " ERROR_FLOAT, t[o + 2]));
	}
	if (count > 3)
	{
		m->emitter = parse_float(t[o + 3], &scene->is_valid);
		if (!scene->is_valid)
			return (print_error("Material: emitter: " ERROR_FLOAT, t[o + 3]));
	}
	if (count > 4)
	{
		m->specular_color = parse_color(t[o + 4], &scene->is_valid);
		if (!scene->is_valid)
			return (print_error("Material: sp_color: " ERROR_COLOR, t[o + 4]));
	}
	return (SUCCESS);
}
