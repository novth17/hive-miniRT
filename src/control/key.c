#include "mini_rt.h"

static void	esc_key_func(mlx_key_data_t keydata, t_fractol *fractol)
{
	if (keydata.key == MLX_KEY_ESCAPE && keydata.action == MLX_PRESS)
	{
		mlx_terminate(fractol->mlx);
		exit(EXIT_SUCCESS);
	}
}

void	key_hook(mlx_key_data_t key_data, void *param)
{
	t_fractol	*fractol;

	fractol = (t_fractol *)param;
	color_mode_hook(key_data, fractol);
	shift_horizontal(key_data, fractol);
	shift_vertical(key_data, fractol);
	esc_key_func(key_data, fractol);
	if (key_data.key == MLX_KEY_KP_SUBTRACT
		&& fractol->max_iter > 5)
	{
		fractol->max_iter *= 0.9;
		if (fractol->max_iter <= 5)
			fractol->max_iter = 5;
	}
	if (key_data.key == MLX_KEY_KP_ADD
		&& fractol->max_iter < 500)
	{
		fractol->max_iter *= 1.2;
		if (fractol->max_iter >= 500)
			fractol->max_iter = 500;
	}
}
