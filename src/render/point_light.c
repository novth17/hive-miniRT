/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   point_light.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiennguy <hiennguy@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 19:13:30 by ltaalas           #+#    #+#             */
/*   Updated: 2025/08/29 18:10:53 by hiennguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

static inline
bool	shadow_hit(const t_scene *restrict scene,
	const t_ray ray, const float light_distance)
{
	uint32_t	i;
	float		hit_distance;

	i = -1;
	while (++i < scene->pl_count)
	{
		hit_distance = plane_hit(scene->pls[i], ray);
		if (hit_distance > MIN_HIT_DIST && hit_distance < light_distance)
			return (true);
	}
	i = -1;
	while (++i < scene->spheres_count)
	{
		hit_distance = sphere_hit(scene->spheres[i], ray);
		if (hit_distance > MIN_HIT_DIST && hit_distance < light_distance)
			return (true);
	}
	i = -1;
	while (++i < scene->cyls_count)
	{
		hit_distance = cyl_hit(scene->cyls[i], ray);
		if (hit_distance > MIN_HIT_DIST && hit_distance < light_distance)
			return (true);
	}
	return (false);
}

static inline
t_v3	reflect(t_v3 incident, t_v3 normal)
{
	t_v3		result;
	const float	two_times_dot = 2.0f * dot(normal, incident);

	result = f32_mul_v3(two_times_dot, normal);
	result = V3_SUB(incident, result);
	return (normalize(result));
}

static inline
t_v3	point_light_color(
		const t_light *restrict light,
		const t_hit *restrict rec,
		t_v3 light_direction,
		float dist)
{
	float		light_angle;
	t_color		lambertian;
	t_v3		reflect_dir;
	t_color		specular_color;
	float		specular;

	dist = dist * dist;
	specular_color = v3(0, 0, 0);
	light_angle = smoothstep(0.0f, 1.0f, dot(rec->normal, light_direction));
	lambertian = f32_mul_v3(light_angle, light->color);
	reflect_dir = reflect(light_direction, rec->normal);
	light_angle = dot(reflect_dir, rec->view_direction);
	light_angle = fmaxf(light_angle, 0.0f);
	specular = powf(light_angle, 256.0f);
	specular_color = v3_mul_v3(light->color, rec->mat.specular_color);
	specular_color = v3_mul_f32(specular_color, specular);
	return (v3_clamp(v3_div_f32(V3_ADD(lambertian, specular_color), dist)));
}

inline
t_v3	check_point_light(
		const t_scene *restrict scene,
		const t_hit *restrict rec)
{
	t_color		color;
	t_ray		shadow_ray;
	t_v3		light_direction;
	float		light_dist;
	uint32_t	i;

	i = 0;
	color = v3(0, 0, 0);
	while (i < scene->lights_count)
	{
		light_direction = V3_SUB(scene->lights[i].origin, rec->position);
		light_dist = length(light_direction);
		light_direction = f32_mul_v3(1.0f / light_dist, light_direction);
		shadow_ray.origin = rec->position;
		shadow_ray.direction = light_direction;
		if (shadow_hit(scene, shadow_ray, light_dist) == false)
		{
			light_dist = light_dist / scene->light_dist_mult;
			color = V3_ADD(color, point_light_color(&scene->lights[i], rec,
						light_direction, light_dist));
		}
		i++;
	}
	return (v3_clamp(color));
}
