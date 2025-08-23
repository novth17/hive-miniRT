#include "mini_rt.h"


#define HIT_ONCE 0
#define PREV_SPECULAR 1

static inline
bool find_closest_ray_intesection(t_hit *restrict rec, const t_ray ray, const t_scene * restrict scene)
{
	// t_sphere point_light_sphere = {.material.color = v3(1, 1, 1), .center = scene->light.origin, .radius = 0.05f}; // debugging
	// point_light_sphere.material.emitter = 0.0f;
	// point_light_sphere.material.diffuse = 0.0f;
	// point_light_sphere.material.specular_probability = 0.0f;

	check_planes(rec, scene->pls, scene->pl_count, ray);
	check_spheres(rec, scene->spheres, scene->spheres_count, ray);
	// check_spheres(&hit_record, &point_light_sphere, 1, ray);
	check_cyl(rec, scene->cyls, scene->cyls_count, ray);

	return (rec->did_hit);
}

#define RAY 0
#define PREV 1
#define INCOMING 2
#define EMIT 3
#define TEMP 4

#define IS 0

static inline
t_v4 trace(t_ray ray, const t_scene * restrict scene, const uint32_t max_bounce, uint32_t *seed) // change to all objects or scene;
{
	const t_v3 ambient = f32_mul_v3(scene->ambient.ratio, scene->ambient.color); // put this into scene when parsing
	t_color color[5];
	bool hit_once;
	bool specular[2];
	uint32_t i;
	t_hit rec;

	rec = (t_hit){};

	hit_once = false;
	specular[IS] = false;
	specular[PREV] = true;
	color[RAY] = v3(1, 1, 1);
	color[PREV] = color[RAY];
	color[INCOMING] = v3(0, 0, 0);

	i = 0;
	while (i <= max_bounce)
	{
		rec.distance = MAX_HIT_DIST;
		if (find_closest_ray_intesection(&rec, ray, scene))
		{
			// if we hit something calculate the light contribution of that point into the total light of the ray
			hit_once = true;
			rec.position = V3_ADD(rec.position, v3_mul_f32(rec.normal, 1e-4f));
			specular[IS] = rec.mat.specular_probability >= random_float(seed);
			color[EMIT] = v3_mul_f32(rec.mat.color, rec.mat.emitter);
			color[INCOMING] = V3_ADD(color[INCOMING], v3_mul_v3(color[EMIT], color[RAY]));
			color[RAY] = v3_mul_v3(color[RAY], v3_lerp(rec.mat.color, specular[IS], rec.mat.specular_color));
			if (scene->use_point_light)
			{
				color[INCOMING] = V3_ADD(color[INCOMING], v3_mul_v3(check_point_light(scene, &rec), color[RAY]));
			}

			if (i > 0 && !specular[PREV])
			{
				float p = fmaxf(color[RAY].r, fmaxf(color[RAY].g, color[RAY].b));
				if (random_float(seed) >= p)
				{
					color[RAY] = color[PREV];
					break;
				}
			}
			specular[PREV] = specular[IS];
			color[PREV] = color[RAY];
			ray = calculate_next_ray(&rec, ray, specular[IS], seed);
			++i;
		}
		else
		{

			break ;
		}
	}
	if (hit_once)
	{
		color[INCOMING] = V3_ADD(color[INCOMING], v3_mul_v3(ambient, color[RAY]));
		return ((t_v4){.rgb = v3_clamp(color[INCOMING]), .a = 1.0f});
	}
	return (v4(0, 0, 0, 0));
}


inline
t_v4	sample_pixel(
	const t_scene *restrict scene,
	const t_camera *restrict cam,
	const t_cord original_cord,
	uint32_t seed)
{
	t_ray	ray;
	t_v4	color;
	t_v4	incoming_light;
	int32_t	y_s;
	int32_t	x_s;

	incoming_light = v4(0, 0, 0, 0);
	y_s = 0;
	while (y_s < cam->sqrt_spp)
	{
		x_s = 0;
		while (x_s < cam->sqrt_spp)
		{
			ray = get_ray(cam, original_cord, (t_cord){x_s, y_s}, &seed);
			color = trace(ray, scene, cam->max_bounce, &seed);
			incoming_light = v4_add(incoming_light, color);
			++x_s;
		}
		++y_s;
	}
	color = v4_mul_f32(incoming_light, cam->pixel_sample_scale_strati);
	return (color);
}
