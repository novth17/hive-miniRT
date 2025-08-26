#include "mini_rt.h"

static int	parse_mat_args1(char **t, int count, int o, t_material *m)
{
	bool is_valid;

	is_valid = true;
	if (count > 1)
	{
		m->diffuse = parse_float(t[o + 1], &is_valid);
		if (!is_valid)
			return (print_error("Material: diffuse: " ERROR_FLOAT, t[o + 1]));
	}
	if (count > 2)
	{
		m->specular_probability = parse_float(t[o + 2], &is_valid);
		if (!is_valid)
			return (print_error("Material: specular: " ERROR_FLOAT, t[o + 2]));
	}
	return (SUCCESS);
}

static int	parse_mat_args2(char **t, int count, int o, t_material *m)
{
	bool is_valid;

	is_valid = true;
	if (count > 3)
	{
		m->emitter = parse_float(t[o + 3], &is_valid);
		if (!is_valid)
			return (print_error("Material: emitter: " ERROR_FLOAT, t[o + 3]));
	}
	if (count > 4)
	{
		m->specular_color = parse_color(t[o + 4], &is_valid);
		if (!is_valid)
			return (print_error("Material: sp_color: " ERROR_COLOR, t[o + 4]));
	}
	return (SUCCESS);
}

static int parse_mat_checker(char **t, int count, int o, t_material *m)
{
	if (count > 5)
	{
    	if (ft_strcmp(t[o + 5], "checker") == 0)
        	m->has_checker = 1;
		else
			return (FAIL);
	}
	return (SUCCESS);
}

int	parse_material(char **t, t_scene *scene, t_material *m, int o)
{
	const int	count = ft_count_2d(t) - o;

	m->color = parse_color(t[o], &scene->is_valid);
	if (!scene->is_valid)
		return (print_error("Material: color: " ERROR_COLOR, t[o]));
	m->diffuse = 0.0f;
	m->specular_probability = 0.0f;
	m->emitter = 0.0f;
	m->specular_color = (t_v3){{1.0, 1.0, 1.0}};
	m->has_checker = 0;

	if (parse_mat_args1(t, count, o, m)
		|| parse_mat_args2(t, count, o, m)
		|| parse_mat_checker(t, count, o, m))
	{
		scene->is_valid = false;
		return (FAIL);
	}
	return (SUCCESS);
}
