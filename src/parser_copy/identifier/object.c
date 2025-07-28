#include "mini_rt.h"

static t_object	*create_objects(t_scene *scene, t_obj_type type)
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

	if (check_id_args_count(tokens, "Sphere", 4) == FAIL)
		return (FAIL);
	object = create_objects(scene, SPHERE);
	if (!object)
		return (FAIL);
	object->sphere.center = parse_vec3(tokens[1], &scene->is_valid);
	if (!scene->is_valid)
		return (print_error("Sphere: center: "ERROR_COORD, tokens[1]));
	object->sphere.radius = parse_float(tokens[2], &scene->is_valid) / 2;
	if (!scene->is_valid)
		return (print_error("Sphere: diameter: "ERROR_FLOAT, tokens[2]));
	object->sphere.color = parse_color(tokens[3], &scene->is_valid);
	if (!scene->is_valid)
		return (print_error("Sphere: "ERROR_COLOR, tokens[3]));
	return (SUCCESS);
}

int parse_plane(char **tokens, t_scene *scene)
{
	t_object *object;

	if (check_id_args_count(tokens, "Plane", 4) == FAIL)
		return (FAIL);
	object = create_objects(scene, PLANE);
	if (!object)
		return (FAIL);
	object->pl.point = parse_vec3(tokens[1], &scene->is_valid);
	if (!scene->is_valid)
		return (print_error("Plane: point: "ERROR_COORD, tokens[1]));
	object->pl.axis = parse_vec3(tokens[2], &scene->is_valid);
	if (!scene->is_valid)
		return (print_error("Plane: axis: "ERROR_COORD, tokens[2]));
	if (!is_in_range_vec3(object->pl.axis))
		return (print_error("Plane: axis: "ERROR_IN_RANGE, tokens[2]));
	if(!is_normalized(object->pl.axis))
		return (print_error("Plane: axis: "ERROR_NORM, tokens[2]));
	object->pl.color = parse_color(tokens[3], &scene->is_valid);
	if (!scene->is_valid)
		return (print_error("Plane: "ERROR_COLOR, tokens[3]));
	return (SUCCESS);
}

int parse_cyl(char **tokens, t_scene *scene)
{
	t_object *object;

	if (check_id_args_count(tokens, "Cylinder", 6) == FAIL)
		return (FAIL);
	object = create_objects(scene, CYLINDER);
	if (!object)
		return (FAIL);
	object->cyl.center = parse_vec3(tokens[1], &scene->is_valid);
	if (!scene->is_valid)
		return (print_error("Cylinder: center: "ERROR_COORD, tokens[1]));
	object->cyl.axis = parse_vec3(tokens[2], &scene->is_valid);
	if (!scene->is_valid)
		return (print_error("Cylinder: axis: "ERROR_COORD, tokens[2]));
	if (!is_in_range_vec3(object->cyl.axis))
		return (print_error("Cylinder: axis: "ERROR_IN_RANGE, tokens[2]));
	if(!is_normalized(object->pl.axis))
		return (print_error("Cylinder: axis: "ERROR_NORM, tokens[2]));
	object->cyl.diameter = parse_float(tokens[3], &scene->is_valid);
	if (!scene->is_valid)
		return (print_error("Cylinder: diameter: "ERROR_FLOAT, tokens[3]));
	object->cyl.height = parse_float(tokens[4], &scene->is_valid);
	if (!scene->is_valid)
		return (print_error("Cylinder: height: "ERROR_FLOAT, tokens[4]));
	object->cyl.color = parse_color(tokens[5], &scene->is_valid);
	if (!scene->is_valid)
		return (print_error("Cylinder: "ERROR_COLOR, tokens[5]));
	return (SUCCESS);
}
