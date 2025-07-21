/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiennguy <hiennguy@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 22:28:07 by hiennguy          #+#    #+#             */
/*   Updated: 2025/07/21 16:18:43 by hiennguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"
// #include "../test_stuff/rt_math.c"

// static void	render(t_minirt *minirt)
// {
// float	aspect_ratio = WINDOW_WIDTH / WINDOW_HEIGHT;
// 	int32_t image_width = WINDOW_WIDTH;
// 	int32_t image_height = (int32_t)(image_width / aspect_ratio);
// 	if (image_height < 1)
// 		image_height = 1;

// 	float focal_length = 1.0f;
// 	float viewport_height = 2.0f;
// 	float viewport_width = viewport_height * (float)(image_width) / image_height;
// 	t_v3 camera_center = v3(0, 0, 0);

// 	t_v3 viewport_u = v3(viewport_width, 0, 0);
// 	t_v3 viewport_v = v3(0, -viewport_height, 0);

// 	t_v3 pixel_delta_u = v3_div_f32(viewport_u, (float)image_width);
// 	t_v3 pixel_delta_v = v3_div_f32(viewport_v, (float)image_height);

// 	t_v3 viewport_upper_left = v3_sub_v3(camera_center, (v3_sub_v3(v3_sub_v3(v3(0, 0, focal_length), v3_div_f32(viewport_u, 2.0f)), v3_div_f32(viewport_v, 2.0f))));
// 	t_v3 pixel00_location = v3_add_v3(viewport_upper_left, f32_mul_v3(0.5f, (v3_add_v3(pixel_delta_u, pixel_delta_v))));



// 	t_v3 position = {.x = 0, .y = 0, .z = -1.0f};
// 	float radius = 0.5f;

// 	// t_v3 ray_origin = {.x = 0, .y = 0, .z = 0};
// 	// t_v3 ray_direction;

// 	uint32_t x;
// 	uint32_t y;
// 	y = 0;
// 	while (y < minirt->image->height)
// 	{
// 		x = 0;
// 		while (x < minirt->image->width)
// 		{
// 			t_v3 pixel_center = v3_add_v3(v3_add_v3(pixel00_location, f32_mul_v3((float)y, pixel_delta_u)),  f32_mul_v3((float)x, pixel_delta_v));
// 			t_v3 ray_direction = v3_sub_v3(pixel_center, camera_center);
// 			t_v3 ray_origin = camera_center;
// 			t_v3 oc = v3_sub_v3(position, ray_origin);
// 			// bool hit = 0;
// 			float a = dot(ray_direction, ray_direction);
// 			float b = -2.0f * dot(ray_direction, oc);
// 			float c = dot(oc, oc) - radius*radius;
// 			float discrimanant = b*b - 4*a*c;
// 			printf("a:%f b: %f c: %f d:%f\n", a,b,c,
// 				 discrimanant);
// 			if (discrimanant >= 0)
// 			{
// 				// uint32_t color = rgba_pack();
// 				mlx_put_pixel(minirt->image, x, y, 0x770000FF);
// 			}
// 			else
// 				mlx_put_pixel(minirt->image, x, y, 0xFFFFFFFF + (uint32_t)discrimanant);
// 			++x;
// 		}
// 		++y;
// 	}
// }

static int	run_minirt(t_minirt *minirt, char **argv)
{
	init_minirt(minirt, argv);
	//render(minirt);
	mlx_loop(minirt->mlx);
	mlx_terminate(minirt->mlx);
}

int	main(int argc, char **argv)
{
	t_minirt minirt;
	if (argc != 2)
	{
		ft_dprintf(2, "Argument must be exactly 1 as a file [name].rt, try again\n");
		return (FAIL);
	}
	if (!argv[1])
		exit_error("Usage: ./miniRT scene.rt");
	if (run_minirt(&minirt, argv) == FAIL)
		return (FAIL);
	write(1, "hihihaha\n", 8);
	ft_dprintf(1, "success hihihaha\n");
	return (SUCCESS);
}


