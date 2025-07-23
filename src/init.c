/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiennguy <hiennguy@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 20:02:04 by hiennguy          #+#    #+#             */
/*   Updated: 2025/07/22 20:37:28 by hiennguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "mini_rt.h"

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
	mlx_set_setting(MLX_STRETCH_IMAGE, false);
	mlx_set_window_size(minirt->mlx, 1024, 1024);
	return (SUCCESS);
}

int	init_minirt(t_minirt *minirt, char **argv)
{
	ft_bzero(minirt, sizeof(t_minirt));

	// if (parse_file(minirt, argv[1]) == FAIL)
	// {
	// 	ft_dprintf(2, "Parsing failed");
	// 	return (FAIL);
	// }
	if (setup_mlx(minirt) == FAIL)
	{
		ft_dprintf(2, "Failed to initialize MLX");
		return (FAIL);
	}
	return (SUCCESS);
}
