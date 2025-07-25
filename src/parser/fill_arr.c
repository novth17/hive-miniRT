/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_arr.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiennguy <hiennguy@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 22:07:28 by hiennguy          #+#    #+#             */
/*   Updated: 2025/07/25 22:29:26 by hiennguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "mini_rt.h"

int fill_spheres_arr(t_minirt *minirt, t_scene *scene)
{
	t_object *cur;
	int i;

	cur = scene->objects;
	scene->spheres_count = 0;
	while (cur)
	{
		if (cur->type == SPHERE)
			scene->spheres_count++;
		cur = cur->next;
	}
	if (scene->spheres_count == 0)
		return (FAIL);
	scene->spheres = malloc(sizeof(t_sphere) * scene->spheres_count);
	if (!scene->spheres)
		exit_error(minirt, "Error\nMalloc failed in fill_spheres_arr");
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

