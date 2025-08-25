#include "mini_rt.h"

static int	parse_mat_args1(char **t, t_scene *scene, int o, t_material *m)
{
	const int	count = ft_count_2d(t) - o;

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
	return (SUCCESS);
}

static int	parse_mat_args2(char **t, t_scene *scene, int o, t_material *m)
{
	const int	count = ft_count_2d(t) - o;

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

static int parse_mat_checker(char **t, int o, t_material *m)
{
    const int count = ft_count_2d(t) - o;

    if (count > 5 && ft_strcmp(t[o + 5], "checker") == 0) // why 5?? spere place and cyl have different
        m->has_checker = 1;
    return SUCCESS;
}



int	parse_material(char **t, t_scene *scene, t_material *m, int o)
{
	m->color = parse_color(t[o], &scene->is_valid);
	if (!scene->is_valid)
		return (print_error("Material: color: " ERROR_COLOR, t[o]));
	m->diffuse = 0.0f;
	m->specular_probability = 0.0f;
	m->emitter = 0.0f;
	m->specular_color = (t_v3){{1.0, 1.0, 1.0}};
	m->has_checker = 0;

	if (parse_mat_args1(t, scene, o, m)
		|| parse_mat_args2(t, scene, o, m)
		|| parse_mat_checker(t, o, m))
		return (FAIL);



	return (SUCCESS);
}
