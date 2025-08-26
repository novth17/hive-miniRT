#include "mini_rt.h"

#define RAY 0
#define PREV 1
#define TOTAL 2
#define IS 0

static inline
bool	hit_color(
	t_color *restrict color,
	t_hit *restrict rec,
	const t_scene *restrict scene,
	uint32_t *seed)
{
	t_color	emit_color;
	t_color	mat_color;
	t_color	light_color;
	bool	is_specular;

	is_specular = rec->mat.specular_probability >= random_float(seed);
	rec->position = V3_ADD(rec->position, v3_mul_f32(rec->normal, 1e-4f));
	emit_color = v3_mul_f32(rec->mat.color, rec->mat.emitter);
	emit_color = v3_mul_v3(emit_color, color[RAY]);
	color[TOTAL] = V3_ADD(color[TOTAL], emit_color);
	mat_color = v3_lerp(rec->mat.color, is_specular, rec->mat.specular_color);
	color[RAY] = v3_mul_v3(color[RAY], mat_color);
	if (scene->use_point_light)
	{
		light_color = v3_mul_v3(check_point_light(scene, rec), color[RAY]);
		color[TOTAL] = V3_ADD(color[TOTAL], light_color);
	}
	return (is_specular);
}

static inline
bool	monte_carlo_termin(t_color *restrict color, uint32_t *seed)
{
	float	p;

	p = fmaxf(color[RAY].r, fmaxf(color[RAY].g, color[RAY].b));
	if (random_float(seed) >= p)
	{
		color[RAY] = color[PREV];
		return (true);
	}
	return (false);
}

static inline
void	init_trace_variables(
	t_color *color,
	bool *specular,
	t_hit *rec, uint32_t *i)
{
	*rec = (t_hit){};
	specular[IS] = false;
	specular[PREV] = true;
	color[RAY] = v3(1, 1, 1);
	color[PREV] = color[RAY];
	color[TOTAL] = v3(0, 0, 0);
	*i = 0;
}

static inline
t_v4	trace(t_ray ray,
		const t_scene *restrict scene,
		const uint32_t max_bounce,
		uint32_t *seed)
{
	t_hit		rec;
	t_color		color[3];
	uint32_t	i;
	bool		specular[2];

	init_trace_variables(color, specular, &rec, &i);
	while (i <= max_bounce)
	{
		if (find_closest_ray_intesection(&rec, ray, scene))
		{
			specular[IS] = hit_color(color, &rec, scene, seed);
			if (i > 0 && !specular[PREV] && monte_carlo_termin(color, seed))
				break ;
			specular[PREV] = specular[IS];
			color[PREV] = color[RAY];
			ray = calculate_next_ray(&rec, ray, specular[IS], seed);
			++i;
		}
		else
			break ;
	}
	if (i == 0)
		return (v4(0, 0, 0, 0));
	color[RAY] = v3_mul_v3(color[RAY], scene->ambient.color);
	return (v3_to_v4(v3_clamp(V3_ADD(color[TOTAL], color[RAY])), 1.0f));
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

	seed = (original_cord.y * cam->image_width + original_cord.x)
		+ (seed * 779353);
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
