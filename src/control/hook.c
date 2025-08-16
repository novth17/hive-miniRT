#include "../../inc/mini_rt.h"

static
void set_in_bounds(t_camera *cam)
{
	if (cam->focus_dist < 1.f)
		cam->focus_dist = 1.f;
	if (cam->defocus_angle < 0.0f)
		cam->defocus_angle = 0.0f;
	if (cam->fov > 180.0f)
		cam->fov = 180.0f;
	if (cam->fov < 0.0f)
		cam->fov = 0.0f;
	if (cam->max_bounce < 0)
		cam->max_bounce = 0;
	if (cam->samples_per_pixel < 1)
		cam->samples_per_pixel = 1;
}

void scroll_hook(double delta_x, double delta_y, void *param)
{
	t_minirt *minirt;
	t_camera *cam;
	(void)delta_x;

	if (delta_y == .0)
		return ;
	minirt = (t_minirt *)param;
	cam = &minirt->scene.camera;
	minirt->recalculate_cam = true;
	if (mlx_is_key_down(minirt->mlx, MLX_KEY_LEFT_SHIFT))
	{
		if (delta_y > 0)
			cam->focus_dist += 0.2f;
		else if (delta_y < 0)
			cam->focus_dist -= 0.2f;
	}
	else if (mlx_is_key_down(minirt->mlx, MLX_KEY_LEFT_CONTROL))
	{
		if (delta_y > 0)
			cam->defocus_angle += 0.2f;
		else if (delta_y < 0)
			cam->defocus_angle -= 0.2f;
	}
	else
	{
		if (delta_y > 0)
			cam->fov -= 5.0f;
		else if (delta_y < 0)
			cam->fov += 5.0f;
	}
	set_in_bounds(cam);
}


// maybe just close window and leave from main?
// or maybe this is easier i dunno
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
bool should_recalculate(keys_t key)
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
	if (key == MLX_KEY_EQUAL)
		return (true);
	if (key == MLX_KEY_MINUS)
		return (true);
	if (key == MLX_KEY_PAGE_UP)
		return (true);
	if (key == MLX_KEY_PAGE_DOWN)
		return (true);
	if (key == MLX_KEY_L)
		return (true);
	return (false);
}



void check_movement_keys(keys_t key, t_camera *cam, double time_step)
{
	const float	speed = 0.5f;
	const t_v3	right_direction = v3_mul_f32(cam->right, speed * time_step);
	const t_v3	forward_direction = v3_mul_f32(cam->lookat, speed * time_step);
	const t_v3	up_direction = v3_mul_f32(cam->vup, speed * time_step);

	if (key == MLX_KEY_W)
		cam->lookfrom = V3_ADD(cam->lookfrom, forward_direction);
	else if (key == MLX_KEY_S)
		cam->lookfrom = V3_SUB(cam->lookfrom, forward_direction);
	if (key == MLX_KEY_D)
		cam->lookfrom = V3_ADD(cam->lookfrom, right_direction);
	else if (key == MLX_KEY_A)
		cam->lookfrom = V3_SUB(cam->lookfrom, right_direction);
	if (key == MLX_KEY_E)
		cam->lookfrom = V3_ADD(cam->lookfrom, up_direction);
	else if (key == MLX_KEY_Q)
		cam->lookfrom = V3_SUB(cam->lookfrom, up_direction);
}

void check_other_keys(keys_t key, t_minirt *minirt, t_camera *cam)
{
	if (key == MLX_KEY_P)
		minirt->write_image_to_file = true;
	if (key == MLX_KEY_PAGE_UP)
		cam->max_bounce += 1;
	else if (key == MLX_KEY_PAGE_DOWN)
		cam->max_bounce -= 1;
	if (key == MLX_KEY_EQUAL)
	{
		cam->samples_per_pixel += 1;
	}
	else if (key == MLX_KEY_MINUS)
		cam->samples_per_pixel -= 1;
	if (key == MLX_KEY_L)
		minirt->scene.use_point_light = !minirt->scene.use_point_light;
}


void key_hook(struct mlx_key_data data, void * param)
{
	t_minirt *minirt;

	minirt = (t_minirt *)param;
	if (data.action == MLX_RELEASE)
		return ;

	esc_key_func(data, minirt);
	minirt->recalculate_cam = should_recalculate(data.key);
	check_movement_keys(data.key, &minirt->scene.camera, minirt->mlx->delta_time);
	check_other_keys(data.key, minirt, &minirt->scene.camera);
	set_in_bounds(&minirt->scene.camera);
}
