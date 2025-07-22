/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiennguy <hiennguy@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 22:28:07 by hiennguy          #+#    #+#             */
/*   Updated: 2025/07/22 15:05:44 by hiennguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/mini_rt.h"
#include "../inc/rt_math.h"
#include "../test_stuff/MLX42.h"

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
//
#include "camera.c"
#include <stdint.h>
#include <math.h>

static
float ExactLinearTosRGB(float L)
{
	float S;

	if (L < 0.0f)
	{
		L = 0.0f;
	}

	if (L > 1.0f)
	{
		L = 1.0f;
	}

	S = L * 12.92;
	if (L > 0.0031308)
	{
		S = 1.055F*pow(L, 1.0f/2.4f) - 0.055f;
	}
	return (S);
}

typedef struct
{
	t_v3 origin;
	t_v3 direction;
} t_ray;

static inline
t_v3 ray_color(const t_ray *ray)
{
	t_v3 unit_direction = unit_vector(ray->direction);
	float a = 0.5 * (unit_direction.y + 1.0f);

	t_v3 result;
	result = f32_mul_v3(1.0 - a, v3(1.0, 1.0, 1.0));
	result = v3_add_v3(result, f32_mul_v3(a, v3(0.5, 0.7, 1.0)));
	return (result);
}

static inline
t_ray init_ray(t_camera *cam, int32_t x, int32_t y)
{
	const t_v3 x_delta = f32_mul_v3(x, cam->pixel_delta_u);
	const t_v3 y_delta = f32_mul_v3(y, cam->pixel_delta_v);
	const t_v3 pixel_center = V3_ADD(cam->pixel00_loc, V3_ADD(x_delta, y_delta));
	t_ray ray;

	ray.origin = cam->camera_center;
	ray.direction = V3_SUB(pixel_center, cam->camera_center);
	return (ray);
}

typedef struct
{
	t_v3	position;
	float	radius;
	t_v3 	color;
} t_sphere;

typedef struct
{
	uint32_t count;
	t_sphere *arr;
} t_spheres;

static inline
bool sphere_hit(t_sphere sp, t_ray ray)
{

	t_v3 oc = v3_sub_v3(sp.position, ray.origin);
	// t_v3 color = ray_color(&ray);
	// bool hit = 0;
	float a = dot(ray.direction, ray.direction);
	float b = -2.0f * dot(ray.direction, oc);
	float c = dot(oc, oc) - sp.radius*sp.radius;
	float discrimanant = b*b - 4*a*c;
	if (discrimanant >= 0)
	{
		return true;
	}
	else
		return false;
}

typedef struct
{
	t_v3 position;
	t_v3 color;
} t_hit;

static inline
t_hit check_sphere(t_spheres *spheres, t_ray ray)
{
	t_hit hit;
	int i;

	hit = (t_hit){};
	i = 0;
	while (i < spheres->count)
	{
		if (sphere_hit(spheres->arr[i], ray))
		{
			hit.color = v3(1, 0, 0);//spheres->arr[i].color;
		}
		++i;
	}
	return (hit);
}


static
void render(t_minirt *minirt)
{
	t_v3 position = {.x = 0, .y = 0, .z = -1.0f};
	float radius = 0.5f;

	t_camera cam;
	const int count = 1;

	base_init_cam(minirt, &cam);
	t_spheres spheres;
	t_sphere arr[1];
	spheres.count = 1;
	spheres.arr = arr;
	// t_spheres *spheres = (t_spheres *)malloc(sizeof(t_spheres) + count);
	// spheres->count = count;
	arr[0].position = v3(0, 0, -1);
	arr[0].radius = 0.5f;
	arr[0].color = v3(1, 0, 0);

	int32_t y = 0;
	while (y < cam.image_height)
	{
		int32_t x = 0;
		while (x < cam.image_width)
		{
			t_ray ray;
			ray = init_ray(&cam,  x, y);

			// t_v3 color = ray_color(&ray);
			t_v3 color = {};
			// color = check_sphere(spheres, ray);
			t_hit hit = check_sphere(&spheres, ray);

			color = hit.color;
			// exa
			// linear_to_srgb255((t_v4){.rgb = color, .a = 0xFF});
			// uint32_t bmp_value = rgba_pack4x8(linear_to_srgb255((t_v4){.rgb = color, .a = 0xFF}));
   			int rbyte = (int)(255.999 * color.r);
      		int gbyte = (int)(255.999 * color.g);
        	int bbyte = (int)(255.999 * color.b);
        	uint32_t bmp_value = rbyte << 24 | gbyte << 16 | bbyte << 8 | 0xff;
    		// uint32_t bmp_value = exact_rgba_pack4x8(color);
    		mlx_put_pixel(minirt->image, x, y, bmp_value);

			++x;
		}
		++y;
	}

	// return (0);
}





static int	run_minirt(t_minirt *minirt, char **argv)
{
	if (init_minirt(minirt, argv) == FAIL)
		return (FAIL);
	render(minirt);

	mlx_loop(minirt->mlx);
	mlx_terminate(minirt->mlx);
	return (SUCCESS);
}

int	main(int argc, char **argv)
{
	t_minirt minirt;

	// if (validate_input(argc, argv[1]) == FAIL)
	// 	return (FAIL);
	run_minirt(&minirt, argv);
	ft_dprintf(1, "success hihihaha congrats\n");
	return (SUCCESS);
}
