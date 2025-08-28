#include "mini_rt.h"

void	set_in_bounds(t_camera *cam)
{
	if (cam->focus_dist < 0.2f)
		cam->focus_dist = 0.2f;
	if (cam->defocus_angle < 0.0f)
		cam->defocus_angle = 0.0f;
	if (cam->fov > 180.0f)
		cam->fov = 180.0f;
	if (cam->fov < 0.0f)
		cam->fov = 0.0f;
	if (cam->max_bounce < 0)
		cam->max_bounce = 0;
	if (cam->max_bounce > 256)
		cam->max_bounce = 256;
	if (cam->samples_per_pixel < 1)
		cam->samples_per_pixel = 1;
	if (cam->samples_per_pixel > 65536)
		cam->samples_per_pixel = 65536;
}

bool	should_recalculate(keys_t key)
{
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
	if (key == MLX_KEY_UP)
		return (true);
	if (key == MLX_KEY_DOWN)
		return (true);
	if (key == MLX_KEY_R)
		return (true);
	return (false);
}
