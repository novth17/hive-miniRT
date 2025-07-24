/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delete.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiennguy <hiennguy@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 14:51:29 by hiennguy          #+#    #+#             */
/*   Updated: 2025/07/24 21:18:14 by hiennguy         ###   ########.fr       */
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

void	delete_minirt(t_minirt *minirt)
{
	if (!minirt)
		return;
	if (minirt->tokens)
		ft_free_2d(minirt->tokens);
	delete_objects(minirt->scene.objects);
}

