/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delete.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiennguy <hiennguy@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 14:51:29 by hiennguy          #+#    #+#             */
/*   Updated: 2025/07/26 15:08:52 by hiennguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

static void	delete_objects(t_object *obj)
{
	t_object *tmp;

	while (obj)
	{
		tmp = obj->next;
		free(obj);
		obj = tmp;
	}
}


static void delete_obj_arr(t_minirt *minirt)
{
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
		return;
	delete_obj_arr(minirt);
	delete_objects(minirt->scene.objects);
}

