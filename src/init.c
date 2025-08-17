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
		// mlx_resize_hook(minirt->mlx, &on_resize, minirt);
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

	if (parse_file(minirt, argv[1]) == FAIL)
	{
		ft_dprintf(2, "Parsing failed");
		return (FAIL);
	}

	if (fill_obj_arr(minirt, &minirt->scene) == FAIL)
		return (print_error("Filling object array failed", NULL));

	//TEST
	for (uint32_t i = 0; i < minirt->scene.spheres_count; i++)
	{
		t_sphere *s = &minirt->scene.spheres[i];
		printf("DEBUG: sphere[%u]: center=(%.2f, %.2f, %.2f), diameter=%.2f, color=(%2f, %2f, %2f)\n",
			i,
			s->center.x, s->center.y, s->center.z,
			s->radius,
			s->material.color.r,s->material.color.g, s->material.color.b
		);
		if (1)
		{
			printf("DEBUG: sphere[%u]: diffuse=%.2f, probability=%.2f , emitter=%.2f, spec_color=(%2f, %2f, %2f)\n",
				i,
				s->material.diffuse, s->material.specular_probability, s->material.emitter,
				s->material.specular_color.r,  s->material.specular_color.g, s->material.specular_color.b
			);
		}
	}

	for (uint32_t i = 0; i < minirt->scene.pl_count; i++)
	{
		t_plane *pl = &minirt->scene.pls[i];
		printf("DEBUG: pl[%u]: point=(%.2f, %.2f, %.2f), axis=(%.2f, %.2f, %.2f), color=(%2f, %2f, %2f)\n",
			i,
			pl->point.x, pl->point.y, pl->point.z,
			pl->axis.x, pl->axis.y, pl->axis.z,
			pl->material.color.r, pl->material.color.g, pl->material.color.b
		);
		if (1)
		{
			printf("DEBUG: pl[%u]: diffuse=%.2f, probability=%.2f , emitter=%.2f, spec_color=(%2f, %2f, %2f)\n",
				i,
				pl->material.diffuse, pl->material.specular_probability, pl->material.emitter,
				pl->material.specular_color.r,  pl->material.specular_color.g, pl->material.specular_color.b
			);
		}
	}

	for (uint32_t i = 0; i < minirt->scene.cyls_count; i++)
	{
		t_cylinder *cy = &minirt->scene.cyls[i];
		printf("DEBUG: cyl[%u]: center=(%.2f, %.2f, %.2f), axis=(%.2f, %.2f, %.2f),diameter=%.2f, height=%.2f color=(%2f, %2f, %2f)\n",
			i,
			cy->center.x, cy->center.y, cy->center.z,
			cy->axis.x, cy->axis.y, cy->axis.z,
			cy->diameter, cy->height,
			cy->material.color.r, cy->material.color.g, cy->material.color.b
		);
		if (1)
		{
			printf("DEBUG: cy[%u]: diffuse=%.2f, probability=%.2f , emitter=%.2f, spec_color=(%2f, %2f, %2f)\n",
				i,
				cy->material.diffuse, cy->material.specular_probability, cy->material.emitter, cy->material.specular_color.r,  cy->material.specular_color.g, cy->material.specular_color.b
			);
		}
	}

	//TEST ENDS

	if (setup_mlx(minirt) == FAIL)
	{
		ft_dprintf(2, "Failed to initialize MLX");
		return (FAIL);
	}
	return (SUCCESS);
}
