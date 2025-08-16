#include "mini_rt.h"

static
t_v3 background_ray_direction(const t_camera *cam, float x, float y)
{
	const t_v3 x_delta = f32_mul_v3(x, cam->pixel_delta_u);
	const t_v3 y_delta = f32_mul_v3(y, cam->pixel_delta_v);
	const t_v3 pixel_sample = V3_ADD(cam->pixel00_loc, V3_ADD(x_delta, y_delta));
	t_v3 direction;

	direction = V3_SUB(pixel_sample, cam->camera_center);
	return (unit_vector(direction));
}

static
uint32_t blue_to_white_gradient(t_v3 direction)
{
	const float a = 0.5 * (direction.y + 1.0f);
	t_v3 result;

	result = f32_mul_v3(1.0 - a, v3(1.0, 1.0, 1.0));
	result = v3_add_v3(result, f32_mul_v3(a, v3(0.3, 0.4, 1.0)));
	return(exact_pack((t_v4){.rgb = result, .a = 1.0f}));
}

void draw_background(t_minirt *mrt)
{
	const t_camera *cam = &mrt->scene.camera;
	uint32_t *out;
	t_v3 direction;
	uint32_t x;
	uint32_t y;

	if (mrt->background == NULL)
		return ;
	out = (uint32_t *)mrt->background->pixels;
	y = 0;
	while (y < mrt->background->height)
	{
		x = 0;
		while (x < mrt->background->width)
		{
			direction = background_ray_direction(cam, x, y);


			*out++ = blue_to_white_gradient(direction);
			++x;
		}
		++y;
	}
}

int init_background(t_minirt *minirt)
{
	int32_t instance;
	minirt->background = mlx_new_image(minirt->mlx, WINDOW_WIDTH, WINDOW_HEIGHT);
	if (!minirt->background)
	{
		ft_putstr_fd("miniRT: background image allocation failed\n", 2);
		ft_putstr_fd(mlx_strerror(mlx_errno), 2);
		return (FAIL);
	}
	instance = mlx_image_to_window(minirt->mlx, minirt->background, 0, 0);
	if (instance == -1)
	{
		mlx_close_window(minirt->mlx);
		ft_putstr_fd(mlx_strerror(mlx_errno), 2);
		return (FAIL);
	}
	mlx_set_instance_depth(&minirt->background->instances[instance], -1);
	return (SUCCESS);
}
