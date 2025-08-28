/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiennguy <hiennguy@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 17:25:11 by hiennguy          #+#    #+#             */
/*   Updated: 2025/08/28 17:25:48 by hiennguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

static int	setup_mlx(t_minirt *minirt)
{
	minirt->mlx = mlx_init(WINDOW_WIDTH, WINDOW_HEIGHT, "MiniRay", true);
	if (!minirt->mlx)
	{
		ft_dprintf(2, mlx_strerror(mlx_errno));
		return (FAIL);
	}
	minirt->image = mlx_new_image(minirt->mlx, WINDOW_WIDTH, WINDOW_HEIGHT);
	if (!minirt->image)
	{
		mlx_close_window(minirt->mlx);
		ft_putstr_fd(mlx_strerror(mlx_errno), 2);
		return (FAIL);
	}
	if (mlx_image_to_window(minirt->mlx, minirt->image, 0, 0) == -1)
	{
		mlx_close_window(minirt->mlx);
		ft_putstr_fd(mlx_strerror(mlx_errno), 2);
		return (FAIL);
	}
	init_background(minirt);
	mlx_key_hook(minirt->mlx, &key_hook, minirt);
	mlx_scroll_hook(minirt->mlx, &scroll_hook, minirt);
	mlx_loop_hook(minirt->mlx, &per_frame, minirt);
	return (SUCCESS);
}

int	init_minirt(t_minirt *minirt, char **argv)
{
	ft_bzero(minirt, sizeof(t_minirt));
	minirt->scene.is_valid = true;
	minirt->file_has_content = false;
	minirt->has_camera = false;
	if (parse_file(minirt, argv[1]) == FAIL)
	{
		if (!minirt->file_has_content)
			exit_error(minirt, "File is empty!");
		if (!minirt->has_camera)
			exit_error(minirt, "No camera detected!");
	}
	if (fill_obj_arr(minirt, &minirt->scene) == FAIL)
		return (print_error("Filling object array failed", NULL));
	if (setup_mlx(minirt) == FAIL)
	{
		ft_dprintf(2, "Failed to initialize MLX");
		return (FAIL);
	}
	return (SUCCESS);
}
