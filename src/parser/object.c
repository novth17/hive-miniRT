/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiennguy <hiennguy@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 22:15:36 by hiennguy          #+#    #+#             */
/*   Updated: 2025/07/24 21:30:02 by hiennguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

static t_object	*create_objects(t_scene *scene, t_obj_type type)
{
	t_object *obj;

	obj = malloc(sizeof(t_object));

	if (!obj)
	{
		print_error("malloc failed", NULL);
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

	scene->is_valid = true;

	if (check_id_args_count(tokens, "Sphere", 4) == FAIL)
		return (FAIL);
	object = create_objects(scene, SPHERE);
	if (!object)
		return (FAIL);
	object->sphere.center = parse_vec3(tokens[1], &scene->is_valid);
	if (!scene->is_valid)
		return (print_error("Sphere: Invalid coordinates of the sphere center", tokens[1]));
	object->sphere.diameter = parse_float(tokens[2], &scene->is_valid);
	if (!scene->is_valid)
		return (print_error("Sphere: Invalid sphere diameter", tokens[2]));
	object->sphere.color = parse_color(tokens[3], &scene->is_valid);
	if (!scene->is_valid)
		return (print_error("Sphere: Invalid color format (commas and [0,255])", tokens[3]));
	return (SUCCESS);
}

int parse_plane(char **tokens, t_scene *scene)
{
	t_object *object;

	scene->is_valid = true;

	if (check_id_args_count(tokens, "Plane", 4) == FAIL)
		return (FAIL);
	object = create_objects(scene, PLANE);
	if (!object)
		return (FAIL);
	object->pl.point = parse_vec3(tokens[1], &scene->is_valid);
	if (!scene->is_valid)
		return (print_error("Plane: Invalid coordinates of the plane", tokens[1]));
	object->pl.axis = parse_vec3(tokens[2], &scene->is_valid);
	if (!scene->is_valid)
		return (print_error("Plane: Invalid coordinates of the axis", tokens[2]));
	if (!is_normalized_vec3(object->pl.axis))
		return (print_error("Plane: Axis component must be in range [-1,1]", NULL));
	object->pl.color = parse_color(tokens[3], &scene->is_valid);
	if (!scene->is_valid)
		return print_error("Plane: Invalid color format (commas and [0,255])", tokens[3]);
	return (SUCCESS);
}

int parse_cyl(char **tokens, t_scene *scene)
{
	t_object *object;
	scene->is_valid = true;
	if (check_id_args_count(tokens, "Cylinder", 6) == FAIL)
		return (FAIL);
	object = create_objects(scene, CYLINDER);
	if (!object)
		return (FAIL);
	object->cyl.center = parse_vec3(tokens[1], &scene->is_valid);
	if (!scene->is_valid)
		return (print_error("Cylinder: Invalid coordinates of center", tokens[1]));
	object->cyl.axis = parse_vec3(tokens[2], &scene->is_valid);
	if (!scene->is_valid)
		return (print_error("Cylinder: Invalid coordinates of the axis", tokens[2]));
	if (!is_normalized_vec3(object->cyl.axis))
		return (print_error("Cylinder: Axis component must be in range [-1,1]", NULL));
	object->cyl.diameter = parse_float(tokens[3], &scene->is_valid);
	if (!scene->is_valid)
		return (print_error("Cylinder: Invalid cylinder diameter", tokens[3]));
	object->cyl.height = parse_float(tokens[4], &scene->is_valid);
	if (!scene->is_valid)
		return (print_error("Cylinder: Invalid cylinder height", tokens[4]));
	object->cyl.color = parse_color(tokens[5], &scene->is_valid);
	if (!scene->is_valid)
		return print_error("Cylinder: Invalid color format (commas and [0,255])", tokens[5]);
	return (SUCCESS);
}
