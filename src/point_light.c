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

// look at https://en.wikipedia.org/wiki/Blinn%E2%80%93Phong_reflection_model to make this better
static inline
t_v3	point_light_color(
		const t_light *restrict light,
		const t_hit *restrict rec,
		t_v3 light_direction,
		float dist)
{
	const t_v3	point_light_color = f32_mul_v3(light->bright_ratio, light->color); // move this to parsing
	float		light_angle;
	t_v3		color;

	light_angle = smoothstep(dot(rec->normal, light_direction), 0.0f, 2.0f);
	color = f32_mul_v3(light_angle, point_light_color);
	color = f32_mul_v3(1.0f / (dist * dist), color);
	return (v3_clamp(color));
}

inline
t_v3	check_point_light(
		const t_scene *restrict scene,
		const t_hit *restrict rec)
{
	t_color		color;
	t_ray		shadow_ray;
	t_v3		light_vector;
	float		light_dist;
	uint32_t	i;

	i = 0;
	color = v3(0, 0, 0);
	while (i < scene->lights_count)
	{
		light_vector = V3_SUB(scene->lights[i].origin, rec->position);
		light_dist = length(light_vector);
		light_vector = f32_mul_v3(1.0f / light_dist, light_vector);
		shadow_ray.origin = rec->position;
		shadow_ray.direction = light_vector;
		if (shadow_hit(scene, shadow_ray, light_dist) == false)
		{
			light_dist = light_dist / scene->light_dist_mult;
			color = V3_ADD(color, point_light_color(&scene->lights[i], rec,
						light_vector, light_dist));
		}
		i++;
	}
	return (v3_clamp(color));
}
