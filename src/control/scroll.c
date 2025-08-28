/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scroll.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 19:15:03 by ltaalas           #+#    #+#             */
/*   Updated: 2025/08/28 19:15:04 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

static
bool	change_defocus(mlx_t *mlx, t_camera *cam, double delta_y)
{
	if (mlx_is_key_down(mlx, MLX_KEY_LEFT_SHIFT))
	{
		if (delta_y > 0)
			cam->focus_dist += 0.2f;
		else if (delta_y < 0)
			cam->focus_dist -= 0.2f;
		return (true);
	}
	else if (mlx_is_key_down(mlx, MLX_KEY_LEFT_CONTROL))
	{
		if (delta_y > 0)
			cam->defocus_angle += 0.2f;
		else if (delta_y < 0)
			cam->defocus_angle -= 0.2f;
		return (true);
	}
	return (false);
}

void	scroll_hook(double delta_x, double delta_y, void *param)
{
	t_minirt	*minirt;
	t_camera	*cam;

	(void)delta_x;
	if (delta_y == .0)
		return ;
	minirt = (t_minirt *)param;
	cam = &minirt->scene.camera;
	minirt->recalculate_cam = true;
	if (!change_defocus(minirt->mlx, cam, delta_y))
	{
		if (delta_y > 0)
			cam->fov -= 5.0f;
		else if (delta_y < 0)
			cam->fov += 5.0f;
	}
	set_in_bounds(cam);
}
