/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiennguy <hiennguy@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 20:02:04 by hiennguy          #+#    #+#             */
/*   Updated: 2025/07/25 22:33:27 by hiennguy         ###   ########.fr       */
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

<<<<<<< HEAD
	// if (parse_file(minirt, argv[1]) == FAIL)
	// {
	// 	ft_dprintf(2, "Parsing failed");
	// 	return (FAIL);
	// }
=======
	minirt->scene.is_valid = true;

	if (parse_file(minirt, argv[1]) == FAIL)
	{
		ft_dprintf(2, "Parsing failed");
		return (FAIL);
	}
<<<<<<< HEAD
>>>>>>> 9499bfd (fix error new line message)
=======


	if (fill_spheres_arr(minirt, &minirt->scene) == SUCCESS)
	{
		for (uint32_t i = 0; i < minirt->scene.spheres_count; i++)
		{
			t_sphere *s = &minirt->scene.spheres[i];
			ft_dprintf(1,
				"DEBUG: sphere[%u]: center=(%.2f, %.2f, %.2f), diameter=%.2f, color=(%d, %d, %d)\n",
				i,
				s->center.x, s->center.y, s->center.z,
				s->diameter,
				s->color.r, s->color.g, s->color.b
			);
		}
	}



>>>>>>> 1fce0f2 (fill spheres array done)
	if (setup_mlx(minirt) == FAIL)
	{
		ft_dprintf(2, "Failed to initialize MLX");
		return (FAIL);
	}
	return (SUCCESS);
}



