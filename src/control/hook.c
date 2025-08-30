/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hook.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiennguy <hiennguy@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 19:14:47 by ltaalas           #+#    #+#             */
/*   Updated: 2025/08/29 17:36:54 by hiennguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

static float	g_speed = 0.5f;

static void	esc_key_func(mlx_key_data_t keydata, t_minirt *minirt)
{
	if (keydata.key == MLX_KEY_ESCAPE && keydata.action == MLX_PRESS)
	{
		minirt->stop_threads = true;
		mlx_close_window(minirt->mlx);
	}
}

bool	check_movement_keys(t_camera *cam, mlx_t *mlx, double time_step)
{
	const float	speed = g_speed * time_step;
	const t_v3	right_direction = v3_mul_f32(cam->right, speed);
	const t_v3	forward_direction = v3_mul_f32(cam->lookat, speed);
	const t_v3	up_direction = v3_mul_f32(cam->vup, speed);
	int			moved;

	moved = 0;
	if (mlx_is_key_down(mlx, MLX_KEY_W) && ++moved)
		cam->lookfrom = V3_SUB(cam->lookfrom, forward_direction);
	else if (mlx_is_key_down(mlx, MLX_KEY_S) && ++moved)
		cam->lookfrom = V3_ADD(cam->lookfrom, forward_direction);
	if (mlx_is_key_down(mlx, MLX_KEY_D) && ++moved)
		cam->lookfrom = V3_ADD(cam->lookfrom, right_direction);
	else if (mlx_is_key_down(mlx, MLX_KEY_A) && ++moved)
		cam->lookfrom = V3_SUB(cam->lookfrom, right_direction);
	if (mlx_is_key_down(mlx, MLX_KEY_E) && ++moved)
		cam->lookfrom = V3_ADD(cam->lookfrom, up_direction);
	if (mlx_is_key_down(mlx, MLX_KEY_SPACE) && ++moved)
		cam->lookfrom = V3_ADD(cam->lookfrom, up_direction);
	else if (mlx_is_key_down(mlx, MLX_KEY_Q) && ++moved)
		cam->lookfrom = V3_SUB(cam->lookfrom, up_direction);
	return (moved != 0);
}

void	check_setting_keys2(keys_t key, t_camera *cam)
{
	if (key == MLX_KEY_R)
	{
		cam->lookfrom = cam->base_position;
		cam->lookat = cam->base_direction;
		cam->right = normalize(cross(v3(0, 1, 0), cam->lookat));
		cam->vup = normalize(cross(cam->lookat, cam->right));
		reset_rotation_angles();
	}
}

void	check_setting_keys(keys_t key, t_minirt *minirt, t_camera *cam)
{
	if (key == MLX_KEY_P)
		minirt->write_image_to_file = true;
	if (key == MLX_KEY_PAGE_UP)
		cam->max_bounce += 1;
	else if (key == MLX_KEY_PAGE_DOWN)
		cam->max_bounce -= 1;
	if (key == MLX_KEY_EQUAL)
		cam->samples_per_pixel += 1;
	else if (key == MLX_KEY_MINUS)
		cam->samples_per_pixel -= 1;
	if (key == MLX_KEY_L)
		minirt->scene.use_point_light = !minirt->scene.use_point_light;
	if (key == MLX_KEY_UP)
		minirt->scene.light_dist_mult += 0.25f;
	if (key == MLX_KEY_DOWN)
	{
		minirt->scene.light_dist_mult -= 0.25f;
		if (minirt->scene.light_dist_mult < 1.0f)
			minirt->scene.light_dist_mult = 1.0f;
	}
	if (key == MLX_KEY_RIGHT)
		g_speed += 0.1f;
	if (key == MLX_KEY_LEFT)
		g_speed -= 0.1f;
	check_setting_keys2(key, cam);
}

void	key_hook(struct mlx_key_data data, void *param)
{
	t_minirt	*minirt;

	minirt = (t_minirt *)param;
	if (data.action == MLX_RELEASE)
		return ;
	esc_key_func(data, minirt);
	minirt->recalculate_cam = should_recalculate(data.key);
	check_setting_keys(data.key, minirt, &minirt->scene.camera);
	set_in_bounds(&minirt->scene.camera);
	if (g_speed < 0.1f)
		g_speed = 0.1f;
}
