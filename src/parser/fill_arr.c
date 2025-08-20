#include "mini_rt.h"

static int	fill_lights_arr(t_minirt *minirt, t_scene *scene)
{
	t_object	*cur;
	int			i;

	cur = scene->objects;

	if (scene->lights_count == 0)
		return (FAIL);
	scene->lights = malloc(sizeof(t_light) * scene->lights_count);
	if (!scene->lights)
		exit_error(minirt, "Malloc failed in fill_lights_arr");
	cur = scene->objects;
	i = 0;
	while (cur)
	{
		if (cur->type == LIGHT)
			scene->lights[i++] = cur->light;
		cur = cur->next;
	}
	return (SUCCESS);
}

static int	fill_spheres_arr(t_minirt *minirt, t_scene *scene)
{
	t_object	*cur;
	int			i;

	cur = scene->objects;

	if (scene->spheres_count == 0)
		return (FAIL);
	scene->spheres = malloc(sizeof(t_sphere) * scene->spheres_count);
	if (!scene->spheres)
		exit_error(minirt, "Malloc failed in fill_spheres_arr");
	cur = scene->objects;
	i = 0;
	while (cur)
	{
		if (cur->type == SPHERE)
			scene->spheres[i++] = cur->sphere;
		cur = cur->next;
	}
	return (SUCCESS);
}

static int	fill_pls_arr(t_minirt *minirt, t_scene *scene)
{
	t_object	*cur;
	int			i;

	i = 0;

	cur = scene->objects;
	if (scene->pl_count == 0)
		return (FAIL);
	scene->pls = malloc(sizeof(t_plane) * scene->pl_count);
	if (!scene->pls)
		exit_error(minirt, "Malloc failed in fill_pls_arr");
	cur = scene->objects;
	while (cur)
	{
		if (cur->type == PLANE)
			scene->pls[i++] = cur->pl;
		cur = cur->next;
	}
	return (SUCCESS);
}

static int	fill_cyls_arr(t_minirt *minirt, t_scene *scene)
{
	t_object	*cur;
	int			i;

	i = 0;

	cur = scene->objects;

	if (scene->cyls_count == 0)
		return (FAIL);
	scene->cyls = malloc(sizeof(t_cylinder) * scene->cyls_count);
	if (!scene->cyls)
		exit_error(minirt, "Malloc failed in fill_cyls_arr");
	cur = scene->objects;
	while (cur)
	{
		if (cur->type == CYLINDER)
			scene->cyls[i++] = cur->cyl;
		cur = cur->next;
	}
	return (SUCCESS);
}

int	fill_obj_arr(t_minirt *minirt, t_scene *scene)
{
	int	result;

	result = FAIL;
	if (fill_lights_arr(minirt, scene) == SUCCESS)
		result = SUCCESS;
	if (fill_spheres_arr(minirt, scene) == SUCCESS)
		result = SUCCESS;
	if (fill_pls_arr(minirt, scene) == SUCCESS)
		result = SUCCESS;
	if (fill_cyls_arr(minirt, scene) == SUCCESS)
		result = SUCCESS;
	return (result);
}
