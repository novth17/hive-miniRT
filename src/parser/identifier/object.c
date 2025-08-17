#include "mini_rt.h"

static t_object *create_objects(t_scene *scene, t_obj_type type)
{
	t_object *obj;

	obj = malloc(sizeof(t_object));
	if (!obj)
	{
		print_error(ERROR_MALLOC, NULL);
		scene->is_valid = false;
		return (NULL);
	}
	obj->type = type;
	obj->next = scene->objects;
	scene->objects = obj;
	return (obj);
}

int parse_sphere(char **tokens, t_scene *scene)
{
	t_object *object;

	if (check_obj_args_count(tokens, "Sphere", SP_ARG_MIN, SP_ARG_MAX) == FAIL)
		return (FAIL);
	object = create_objects(scene, SPHERE);
	if (!object)
		return (FAIL);
	object->sphere.center = parse_vec3(tokens[1], &scene->is_valid);
	if (!scene->is_valid)
		return (print_error("Sphere: center: " ERROR_COORD, tokens[1]));
	object->sphere.radius = parse_float(tokens[2], &scene->is_valid) / 2;
	if (!scene->is_valid)
		return (print_error("Sphere: diameter: " ERROR_FLOAT, tokens[2]));
	if (parse_material(tokens, scene, &object->sphere.material, 3))
	{
		ft_dprintf(2, RED "Error from Sphere\n" RESET);
		return (FAIL);
	}
	return (SUCCESS);
}

int parse_plane(char **tokens, t_scene *scene)
{
	t_object *object;

	if (check_obj_args_count(tokens, "Plane", PL_ARG_MIN, PL_ARG_MAX) == FAIL)
		return (FAIL);
	object = create_objects(scene, PLANE);
	if (!object)
		return (FAIL);
	object->pl.point = parse_vec3(tokens[1], &scene->is_valid);
	if (!scene->is_valid)
		return (print_error("Plane: point: " ERROR_COORD, tokens[1]));
	object->pl.axis = parse_vec3(tokens[2], &scene->is_valid);
	if (!scene->is_valid)
		return (print_error("Plane: axis: " ERROR_COORD, tokens[2]));
	if (!is_in_range_vec3(object->pl.axis))
		return (print_error("Plane: axis: " ERROR_IN_RANGE, tokens[2]));
	if (!is_normalized(object->pl.axis))
		return (print_error("Plane: axis: " ERROR_NORM, tokens[2]));
	if (parse_material(tokens, scene, &object->pl.material, 3))
	{
		ft_dprintf(2, RED "Error from Plane\n" RESET);
		return (FAIL);
	}
	return (SUCCESS);
}

static int parse_cyl_elem(char **tokens, t_scene *scene, t_object *object)
{
	object->cyl.center = parse_vec3(tokens[1], &scene->is_valid);
	if (!scene->is_valid)
		return (print_error("Cylinder: center: " ERROR_COORD, tokens[1]));
	object->cyl.axis = parse_vec3(tokens[2], &scene->is_valid);
	if (!scene->is_valid)
		return (print_error("Cylinder: axis: " ERROR_COORD, tokens[2]));
	if (!is_in_range_vec3(object->cyl.axis))
		return (print_error("Cylinder: axis: " ERROR_IN_RANGE, tokens[2]));
	if (!is_normalized(object->cyl.axis))
		return (print_error("Cylinder: axis: " ERROR_NORM, tokens[2]));
	object->cyl.diameter = parse_float(tokens[3], &scene->is_valid);
	if (!scene->is_valid)
		return (print_error("Cylinder: diameter: " ERROR_FLOAT, tokens[3]));
	object->cyl.height = parse_float(tokens[4], &scene->is_valid);
	if (!scene->is_valid)
		return (print_error("Cylinder: height: " ERROR_FLOAT, tokens[4]));
	return (SUCCESS);
}

int parse_cyl(char **tokens, t_scene *scene)
{
	t_object *object;

	if (check_obj_args_count(tokens, "Cylinder", CYL_ARG_MIN, CYL_ARG_MAX))
		return (FAIL);
	object = create_objects(scene, CYLINDER);
	if (!object)
		return (FAIL);
	if (parse_cyl_elem(tokens, scene, object))
	{
		ft_dprintf(2, RED "Error from Cylinder element\n" RESET);
		return (FAIL);
	}
	if (parse_material(tokens, scene, &object->cyl.material, 5))
	{
		ft_dprintf(2, RED "Error from Cylinder\n" RESET);
		return (FAIL);
	}
	return (SUCCESS);
}
