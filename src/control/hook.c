#include "../../inc/mini_rt.h"

extern bool g_recalculate_cam;

void scroll_hook(double delta_x, double delta_y, void *param)
{
	t_minirt *minirt;
	(void)delta_x;

	minirt = (t_minirt *)param;
	if (delta_y != 0.0)
		g_recalculate_cam = true;
	if (mlx_is_key_down(minirt->mlx, MLX_KEY_LEFT_SHIFT))
	{
		if (delta_y > 0)
			minirt->scene.camera.focus_dist += 0.2f;
		else if (delta_y < 0)
		{
			minirt->scene.camera.focus_dist -= 0.2f;
			if (minirt->scene.camera.focus_dist < 0.02f)
				minirt->scene.camera.focus_dist = 0.02f;
		}
	}
	else if (mlx_is_key_down(minirt->mlx, MLX_KEY_LEFT_CONTROL))
	{
		if (delta_y > 0)
			minirt->scene.camera.defocus_angle += 0.2f;
		else if (delta_y < 0)
		{
			minirt->scene.camera.defocus_angle -= 0.2f;
			if (minirt->scene.camera.defocus_angle < 0.02f)
			minirt->scene.camera.defocus_angle = 0.02f;
		}
	}
	else
	{
		if (delta_y > 0 && minirt->scene.camera.fov > 5.0f)
			minirt->scene.camera.fov -= 5.0f;
		else if (delta_y < 0 && minirt->scene.camera.fov < 175.0f)
		{
			minirt->scene.camera.fov += 5.0f;
		}
	}
}

static void	esc_key_func(mlx_key_data_t keydata, t_minirt *minirt)
{
	if (keydata.key == MLX_KEY_ESCAPE && keydata.action == MLX_PRESS)
	{
		mlx_terminate(minirt->mlx);
		delete_minirt(minirt);
		exit(EXIT_SUCCESS);
	}
}

static inline
bool is_valid_key(keys_t key)
{
	if (key == MLX_KEY_W)
		return (true);
	if (key == MLX_KEY_S)
		return (true);
	if (key == MLX_KEY_A)
		return (true);
	if (key == MLX_KEY_D)
		return (true);
	if (key == MLX_KEY_E)
		return (true);
	if (key == MLX_KEY_Q)
		return (true);
	if (key == MLX_KEY_P)
		return (true);
	if (key == MLX_KEY_EQUAL)
		return (true);
	if (key == MLX_KEY_MINUS)
		return (true);
	return (false);
}

void key_hook(struct mlx_key_data data, void * param)
{
	const float speed = 0.5f;
	t_minirt *minirt;
	t_v3 right_direction;
	t_v3 forward_direction;
    t_v3 up_direction;

	minirt = (t_minirt *)param;
	right_direction = f32_mul_v3(minirt->mlx->delta_time * speed, minirt->scene.camera.right);
	forward_direction = f32_mul_v3(minirt->mlx->delta_time * speed, minirt->scene.camera.lookat);
	up_direction = f32_mul_v3(minirt->mlx->delta_time * speed, minirt->scene.camera.vup);
	// printf("keyhookend at %f\n", minirt->mlx->delta_time);
	// if (mlx_is_mouse_down(minirt->mlx, MLX_MOUSE_BUTTON_RIGHT))
	if (data.action != MLX_RELEASE && is_valid_key(data.key))
	{
		minirt->recalculate_cam = true;
		// direction = f32_mul_v3(minirt->mlx->delta_time * speed, minirt->scene.camera.lookat);
		if (data.key == MLX_KEY_W)
		{
			minirt->scene.camera.lookfrom = V3_ADD(minirt->scene.camera.lookfrom, forward_direction);
		}
		else if (data.key == MLX_KEY_S)
		{
			minirt->scene.camera.lookfrom = V3_SUB(minirt->scene.camera.lookfrom, forward_direction);
		}
		if (data.key == MLX_KEY_D)
		{
			minirt->scene.camera.lookfrom = V3_ADD(minirt->scene.camera.lookfrom, right_direction);
		}
		else if (data.key == MLX_KEY_A)
		{
			minirt->scene.camera.lookfrom = V3_SUB(minirt->scene.camera.lookfrom, right_direction);
		}
		if (data.key == MLX_KEY_E)
		{
			minirt->scene.camera.lookfrom = V3_ADD(minirt->scene.camera.lookfrom, up_direction);
		}
		else if (data.key == MLX_KEY_Q)
		{
			minirt->scene.camera.lookfrom = V3_SUB(minirt->scene.camera.lookfrom, up_direction);
		}
		if (data.key == MLX_KEY_P)
		{
			minirt->write_image_to_file = true;
		}
		if (data.key == MLX_KEY_EQUAL)
		{
			minirt->scene.camera.max_bounce += 1;
		}
		else if (data.key == MLX_KEY_MINUS)
		{
			if (minirt->scene.camera.max_bounce - 1 > 0)
				minirt->scene.camera.max_bounce -= 1;
		}
	}
	esc_key_func(data, minirt);
}
