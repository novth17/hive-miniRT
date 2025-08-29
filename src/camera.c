/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiennguy <hiennguy@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 19:13:07 by ltaalas           #+#    #+#             */
/*   Updated: 2025/08/29 14:10:19 by hiennguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

float	deg_to_rad(float a)
{
	return (a * M_PI / 180.0f);
}

static inline
t_v3	viewport_top_left(const t_camera *cam, const t_v3 w)
{
	const t_v3	half_u = v3_div_f32(cam->viewport_u, 2.0f);
	const t_v3	half_v = v3_div_f32(cam->viewport_v, 2.0f);
	const t_v3	focus_mul_w = f32_mul_v3(cam->focus_dist, w);
	t_v3		result;

	result = V3_SUB(cam->camera_center, focus_mul_w);
	result = V3_SUB(result, half_u);
	result = V3_SUB(result, half_v);
	return (result);
}

static inline
t_v3	pixel00_location(const t_camera *cam, const t_v3 w)
{
	const t_v3	viewport_upper_left = viewport_top_left(cam, w);
	const t_v3	u_add_v = v3_add_v3(cam->pixel_delta_u, cam->pixel_delta_v);
	t_v3		result;

	result = f32_mul_v3(0.5, u_add_v);
	result = v3_add_v3(viewport_upper_left, result);
	return (result);
}

// to set defaults
void	base_init_cam(t_camera *cam)
{
	cam->samples_per_pixel = 1;
	cam->pixel_sample_scale = 1.0f / cam->samples_per_pixel;
	cam->sqrt_spp = (int32_t)square_root(cam->samples_per_pixel);
	cam->pixel_sample_scale_strati = 1.0f / (cam->sqrt_spp * cam->sqrt_spp);
	cam->recip_sqrt_spp = 1.0f / cam->sqrt_spp;
	cam->max_bounce = MAX_BOUNCE_DEFAULT;
	cam->vup = v3(0, 1, 0);
	cam->base_position = cam->lookfrom;
	cam->base_direction = neg(cam->lookat);
	rotate_view(cam);
	cam->defocus_angle = 0.0f;
	cam->focus_dist = 1.0f;
}

void	init_camera_for_frame(t_minirt *minirt, t_camera *cam)
{
	const float	h = tanf(deg_to_rad(cam->fov) / 2.0f);
	const float	defocus_radius = cam->focus_dist
		* tanf(deg_to_rad(cam->defocus_angle / 2.0f));

	cam->camera_center = cam->lookfrom;
	cam->aspect_ratio = (float)minirt->image->width / minirt->image->height;
	cam->image_width = minirt->image->width;
	cam->image_height = (int32_t)(cam->image_width / cam->aspect_ratio);
	if (cam->image_height < 1)
		cam->image_height = 1;
	cam->viewport_height = 2.0f * h * cam->focus_dist;
	cam->viewport_width = cam->viewport_height
		* ((float)cam->image_width / cam->image_height);
	cam->viewport_u = f32_mul_v3(cam->viewport_width, cam->right);
	cam->viewport_v = f32_mul_v3(cam->viewport_height, neg(cam->vup));
	cam->pixel_delta_u = v3_div_f32(cam->viewport_u, (float)cam->image_width);
	cam->pixel_delta_v = v3_div_f32(cam->viewport_v, (float)cam->image_height);
	cam->pixel00_loc = pixel00_location(cam, cam->lookat);
	cam->defocus_disk_u = v3_mul_f32(cam->right, defocus_radius);
	cam->defocus_disk_v = v3_mul_f32(cam->vup, defocus_radius);
	cam->pixel_sample_scale = 1.0f / cam->samples_per_pixel;
	cam->sqrt_spp = (int32_t)square_root(cam->samples_per_pixel);
	cam->pixel_sample_scale_strati = 1.0f / (cam->sqrt_spp * cam->sqrt_spp);
	cam->recip_sqrt_spp = 1.0f / cam->sqrt_spp;
}
