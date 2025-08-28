/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delete.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiennguy <hiennguy@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 17:24:38 by hiennguy          #+#    #+#             */
/*   Updated: 2025/08/28 17:24:40 by hiennguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

static void	delete_objects(t_object *obj)
{
	t_object	*tmp;

	while (obj)
	{
		tmp = obj->next;
		free(obj);
		obj = tmp;
	}
}

static void	delete_obj_arr(t_minirt *minirt)
{
	if (minirt->scene.lights)
		free(minirt->scene.lights);
	if (minirt->scene.spheres)
		free(minirt->scene.spheres);
	if (minirt->scene.pls)
		free(minirt->scene.pls);
	if (minirt->scene.cyls)
		free(minirt->scene.cyls);
}

void	delete_minirt(t_minirt *minirt)
{
	if (!minirt)
		return ;
	delete_obj_arr(minirt);
	delete_objects(minirt->scene.objects);
	if (minirt->queue.tasks != NULL
		&& minirt->queue.tasks != get_default_task())
	{
		free(minirt->queue.tasks);
		minirt->queue.tasks = NULL;
	}
	free(minirt->linear_color_buf);
}
