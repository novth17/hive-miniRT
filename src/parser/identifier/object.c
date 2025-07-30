#include "mini_rt.h"

#include "mini_rt.h"

static int parse_material(char **tokens, t_scene *scene, t_material *m, int offset);

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

	if (check_obj_args_count(tokens, "Sphere", 7) == FAIL)
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
		return (print_error(RED "Sphere\n", NULL));
	return (SUCCESS);
}

int parse_plane(char **tokens, t_scene *scene)
{
	t_object *object;

	if (check_obj_args_count(tokens, "Plane", 7) == FAIL)
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
		return (print_error(RED "Plane\n", NULL));
	return (SUCCESS);
}

int parse_cyl(char **tokens, t_scene *scene)
{
	t_object *object;

	if (check_obj_args_count(tokens, "Cylinder", 9) == FAIL)
		return (FAIL);
	object = create_objects(scene, CYLINDER);
	if (!object)
		return (FAIL);
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
	if (parse_material(tokens, scene, &object->cyl.material, 5))
		return (print_error(RED "Cylinder\n", NULL));
	return (SUCCESS);
}

static int parse_material(char **tokens, t_scene *scene, t_material *m, int offset)
{
	m->color = parse_color(tokens[offset], &scene->is_valid);
	if (!scene->is_valid)
		return print_error("Material: " ERROR_COLOR, tokens[offset]);
	if (tokens[offset + 1] && tokens[offset + 2] && tokens[offset + 3])
	{
		m->diffuse = parse_float(tokens[offset + 1], &scene->is_valid);
		if (!scene->is_valid)
			return print_error("Material: diffuse: "ERROR_FLOAT, tokens[offset + 1]);

		m->specular_probability = parse_float(tokens[offset + 2], &scene->is_valid);
		if (!scene->is_valid)
			return print_error("Material: specular: "ERROR_FLOAT, tokens[offset + 2]);

		m->emitter = parse_float(tokens[offset + 3], &scene->is_valid);
		if (!scene->is_valid)
			return print_error("Material: emitter: "ERROR_FLOAT, tokens[offset + 3]);
	}
	else
	{
		m->diffuse = 0.0f;
		m->specular_probability = 0.0f;
		m->emitter = 0.0f;
	}
	return (SUCCESS);
}
