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
	t_v3 result;
	const float two_times_dot = 2.0f * dot(normal, incident);

	result = f32_mul_v3(two_times_dot, normal);
	result = V3_SUB(incident, result);
	return (normalize(result));
}

t_v3 schlick_fresnel(t_v3 r0, float light_angle)
{
	const float f0 = 1.0f - light_angle;
	const t_v3 one_minus_r0 = v3_sub_v3(v3(1, 1, 1), r0);

	return (v3_add_v3(r0, v3_mul_f32(one_minus_r0, (f0*f0*f0*f0*f0))));
}

// look at https://en.wikipedia.org/wiki/Blinn%E2%80%93Phong_reflection_model
// to make this better
static inline
t_v3	point_light_color(
		const t_light *restrict light,
		const t_hit *restrict rec,
		t_v3 light_direction,
		float dist)
{
	float		light_angle;
	t_color		lambertian;
	t_v3		half_direction;
	t_color		specular_color;
	const float m = rec->mat.smoothness * 256.0f;

	dist = dist * dist;
	specular_color = v3(0, 0, 0);
	light_angle = smoothstep(0.0f, 1.0f, dot(rec->normal, light_direction));
	// light_angle = fmaxf(dot(rec->normal, light_direction), 0.0f);//, FLT_MAX);
	lambertian = f32_mul_v3(light_angle, light->color);
	lambertian = f32_mul_v3(1.0f / dist, lambertian);
	// return(lambertian);
	// if (is_specular)
	// {
		// BLINN PHON
			// float spec_prob = rec->mat.specular_probability;
			// t_v3 r0 = v3(spec_prob, spec_prob, spec_prob);
			// half_direction = normalize(V3_SUB(light_direction, rec->view_direction));
			// float half_light_angle = fmaxf(dot(half_direction, rec->normal), 0.0f);
			// float roughnessFactor = ((m + 8.0f) * powf(half_light_angle, m)) / 8.0f;
			// float angle_thing = fmaxf(dot(half_direction, light_direction), 0.0f);
			// t_v3 fresnel = schlick_fresnel(r0, angle_thing);
			// t_v3 spec_albedo = v3_mul_f32(fresnel, roughnessFactor);
			// spec_albedo = v3_mul_v3(spec_albedo, f32_div_v3(1.0f, v3_add_f32(spec_albedo, 1.0f)));

			// float light_strength = 20.0f;
			// spec_albedo = v3_mul_f32(spec_albedo, light_strength);
			// specular_color = spec_albedo;


			// light_angle = smoothstep(0.0f, 1.0f, dot(half_direction, rec->normal));
			// float specular = powf(light_angle, m);
			// float specular = roughnessFactor;
			// specular_color = v3_mul_v3(spec_albedo, light->color);

		// PHONG
		// t_v3 reflect_dir = reflect(light_direction, rec->normal);
		// // spec_angle = smoothstep(0.0f, 2.0f, dot(reflect_dir, neg(rec->view_direction)));
		// float spec_angle = dot(reflect_dir, rec->view_direction);
		// // spec_angle = light_angle * 0.5f + 0.5f;
		// spec_angle = fmaxf(spec_angle, 0.0f);
		// float specular = powf(spec_angle, 4.0f);



	// }

	// t_color total = V3_ADD(lambertian, specular_color);
	// total = v3_mul_f32(total, light_strength);
	// total = f32_mul_v3(1.0f / dist, total);
	// return (v3_clamp(total));

	half_direction = normalize(V3_ADD(light_direction, neg(rec->view_direction)));

    // vec3 halfDir = normalize(lightDir + viewDir);
    float specAngle = fmax(dot(half_direction, rec->normal), 0.0);
    float specular = pow(specAngle, m);
		specular_color = f32_mul_v3(specular, light->color);
		specular_color = f32_mul_v3(20.0f, specular_color);
		specular_color = f32_mul_v3(1.0f / dist, specular_color);

	return (v3_clamp(V3_ADD(lambertian, specular_color)));
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
