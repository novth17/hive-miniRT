/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiennguy <hiennguy@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 20:02:04 by hiennguy          #+#    #+#             */
/*   Updated: 2025/07/26 16:58:34 by hiennguy         ###   ########.fr       */
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
	mlx_set_setting(MLX_STRETCH_IMAGE, true);
	mlx_set_window_size(minirt->mlx, 1024, 1024);
	return (SUCCESS);
}

int	init_minirt(t_minirt *minirt, char **argv)
{
	ft_bzero(minirt, sizeof(t_minirt));

	minirt->scene.is_valid = true;
	minirt->file_has_content = false;

	if (parse_file(minirt, argv[1]) == FAIL)
		return (print_error("Parsing failed", NULL));

	if (fill_obj_arr(minirt, &minirt->scene) == FAIL)
		return (print_error("Filling object array failed", NULL));


	#include <stdio.h>
	for (uint32_t i = 0; i < minirt->scene.spheres_count; i++)
	{
		t_sphere *s = &minirt->scene.spheres[i];
		printf("DEBUG: sphere[%u]: center=(%.2f, %.2f, %.2f), diameter=%.2f, color=(%2f, %2f, %2f)\n",
			i,
			s->center.x, s->center.y, s->center.z,
			s->diameter,
			s->color.r, s->color.g, s->color.b
		);
	}

	for (uint32_t i = 0; i < minirt->scene.pl_count; i++)
	{
		t_plane *pl = &minirt->scene.pls[i];
		printf("DEBUG: pl[%u]: point=(%.2f, %.2f, %.2f), axis=(%.2f, %.2f, %.2f), color=(%2f, %2f, %2f)\n",
			i,
			pl->point.x, pl->point.y, pl->point.z,
			pl->axis.x, pl->axis.y, pl->axis.z,
			pl->color.r, pl->color.g, pl->color.b
		);
	}

	for (uint32_t i = 0; i < minirt->scene.cyls_count; i++)
	{
		t_cylinder *cy = &minirt->scene.cyls[i];
		printf("DEBUG: cyl[%u]: center=(%.2f, %.2f, %.2f), axis=(%.2f, %.2f, %.2f),diameter=%.2f, height=%.2f color=(%2f, %2f, %2f)\n",
			i,
			cy->center.x, cy->center.y, cy->center.z,
			cy->axis.x, cy->axis.y, cy->axis.z,
			cy->diameter, cy->height,
			cy->color.r, cy->color.g, cy->color.b
		);
	}

	if (setup_mlx(minirt) == FAIL)
	{
		ft_dprintf(2, "Failed to initialize MLX");
		return (FAIL);
	}
	return (SUCCESS);
}



