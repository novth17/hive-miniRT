# include "mini_rt.h"

extern bool g_recalculate_cam;
extern uint32_t g_accummulated_frames;




#define A x
#define H y
#define C z

static inline
t_hit create_sphere_hit_record(const t_ray ray, const t_sphere sp, const float root)
{
	t_hit rec;

	rec.did_hit = true;
	rec.distance = root;
	rec.position = at(ray, rec.distance);
	rec.color = sp.material.color;
	rec.normal = v3_div_f32(V3_SUB(rec.position, sp.center), sp.radius);
	rec.front_face = dot(ray.direction, rec.normal) < 0;
	if (rec.front_face == false)
	{
		rec.normal = neg(rec.normal);
	}
	return (rec);
}

static inline
bool sphere_hit(t_hit *rec, const t_sphere sp, const t_ray ray)
{
	const t_v3 oc = v3_sub_v3(sp.center, ray.origin);
	const t_v3 v = {
			.A = dot(ray.direction, ray.direction), 	// const float a = dot(ray.direction, ray.direction);
			.H = dot(ray.direction, oc), 				// const float h = dot(ray.direction, oc);
			.C = dot(oc, oc) - sp.radius*sp.radius}; 	// const float c = dot(oc, oc) - sp.radius*sp.radius;
	const float discrimanant = v.H*v.H - v.A*v.C;
	float sqrtd;
	float root;

	if (discrimanant < 0)
		return (false);
	sqrtd = square_root(discrimanant);
	root = (v.H - sqrtd) / v.A;
	if (root <= MIN_HIT_DIST || root >= MAX_HIT_DIST)
	{
		root = (v.H + sqrtd) / v.A;
		if (root <= MIN_HIT_DIST || root >= MAX_HIT_DIST)
			return (false);
	}
	*rec = create_sphere_hit_record(ray, sp, root);
	// rec->distance = root;
	// rec->position = at(ray, rec->distance);
	// rec->normal = v3_div_f32(V3_SUB(rec->position, sp.position), sp.radius);
	// rec->color = sp.color; // maybe replace with material or material index?
	// rec->did_hit = true;
	return (true);
}

inline
void set_face_normal(t_hit *rec, const t_ray *r, const t_v3 out_normal)
{
	const bool front_face = dot(r->direction, out_normal) < 0;
	if (front_face)
		rec->normal = out_normal;
	else
		rec->normal = neg(out_normal);
}

static int hit = 0;
static int miss = 0;

inline
bool check_spheres(t_hit *rec, const t_sphere *spheres, const uint32_t count, const t_ray ray)
{
	t_hit temp_rec;
	uint32_t i;

	temp_rec = (t_hit){};
	temp_rec.distance = MAX_HIT_DIST;
	i = 0;
	while (i < count)
	{
		if(sphere_hit(&temp_rec, spheres[i], ray))
		{
			hit++;
			if (temp_rec.distance < rec->distance)
			{
				*rec = temp_rec; // @QUESTION when to use set_face_normal - here or later?
			}
		}
		miss++;
		// if (t > rec->distance)
		// {
		// 	hit.distance = t; hit.color = v3(1, 0, 0);//spheres->arr[i].color;
		// }
		// else
		// {
		// 	hit.color = ray_color(&ray);
		// }
		++i;
	}
	return (temp_rec.did_hit);
}

static inline
bool shadow_hit(const t_scene *scene, const t_ray ray) // change to all objects or scene;
{
	t_hit temp_rec;
	uint32_t i;

	temp_rec.distance = FLT_MAX;
	i = 0;
	while (i < scene->spheres_count)
	{
		if(sphere_hit(&temp_rec, scene->spheres[i], ray))
		{
			return (true);
		}
		++i;
	}
	return (false);
}


t_v3 defocus_disk_sample(const t_camera *restrict cam, uint32_t *rng_state)
{
	const t_point3 point = random_in_unit_disk(rng_state);
	t_v3 result;

	result = V3_ADD(cam->camera_center, f32_mul_v3(point.x, cam->defocus_disk_u));
	result = V3_ADD(result, f32_mul_v3(point.y, cam->defocus_disk_v));
	return (result);
}




static inline
t_v3 get_ray_direction(const t_camera *cam, const t_ray ray, const t_cord cord, uint32_t *seed)
{
	const t_v3 offset = sample_square(seed);
	// printf("offset x <%f> offset y <%f>\n", offset.x, offset.y);
	// t_v3 offset = {0, 0, 0};
	const t_v3 x_delta = f32_mul_v3(cord.x + offset.x, cam->pixel_delta_u);
	const t_v3 y_delta = f32_mul_v3(cord.y + offset.y, cam->pixel_delta_v);
	const t_v3 pixel_sample = V3_ADD(cam->pixel00_loc, V3_ADD(x_delta, y_delta));

	return (V3_SUB(pixel_sample, ray.origin));
}

inline
float smoothstep(const float edge0, const float edge1, float x)
{
	x = clamp((x - edge0) / (edge1 - edge0), edge0, edge1);
	return (x * x * (3.0f - 2.0f * x));
}

static inline
t_v3 v3_smoothstep(const t_v3 value)
{
	t_v3 result;

	result.x = smoothstep(value.x, 0.0f, 1.0f);
	result.y = smoothstep(value.x, 0.0f, 1.0f);
	result.z = smoothstep(value.x, 0.0f, 1.0f);
	return (result);
}

static inline
t_v3 trace(t_ray ray, const t_scene *scene, uint32_t *seed) // change to all objects or scene;
{

	t_v3 ambient = f32_mul_v3(scene->ambient.ratio, scene->ambient.color);
	// t_v3 point_ligth_loc = v3(2, 2, 2);
	t_v3 point_light_color = f32_mul_v3(scene->light.bright_ratio * 100, scene->light.color);
	t_sphere point_light_sphere = {.material.color = v3(20, 20, 20), .center = scene->light.origin, .radius = 0.05f};
	// t_spheres point_ligth_visualization = {.count = 1, .arr = &point_ligth_spehre};

	// t_new_light p_light = {point_ligth_loc, point_light_color};
	// bounces mby here at some point
	t_hit rec;
	//check_planes(&rec, planes, ray);
	rec = (t_hit){};
	rec.distance = MAX_HIT_DIST;
	check_spheres(&rec, scene->spheres, scene->spheres_count, ray);
	check_spheres(&rec, &point_light_sphere, 1, ray);

	if (rec.did_hit)
	{
		t_v3 l =  V3_SUB(scene->light.origin, rec.position);
		t_ray shadow_ray;
		shadow_ray.origin = V3_ADD(rec.position, v3_mul_f32(rec.normal, 1e-4));
		shadow_ray.direction = l;
		if (shadow_hit(scene, shadow_ray) == false)
		{
			float dist = length(l);
			l = f32_mul_v3(1.0f / dist, l);
			// float light_angle = clamp(dot(rec.normal, l), 0.0f, FLT_MAX);
			float light_angle = smoothstep(dot(rec.normal, l), 0.0f, 2.0f);
			// float light_angle = dot(rec.normal, l);

			t_v3 color = f32_mul_v3(light_angle, point_light_color);
			color = f32_mul_v3(1.0f / (dist * dist), color);
			color = V3_ADD(ambient, color);
			color = v3_mul_v3(rec.color, color);
			return (color);
		}
		return (v3_mul_v3(ambient, rec.color));
	}




	// not hit = background color
	t_v3 unit_direction = unit_vector(ray.direction);
	float a = 0.5 * (unit_direction.y + 1.0f);

	t_v3 result;
	result = f32_mul_v3(1.0 - a, v3(1.0, 1.0, 1.0));
	result = v3_add_v3(result, f32_mul_v3(a, v3(0.5, 0.7, 1.0)));
	// incoming_ligth = result; // for now
	return (result);
}

t_v3 sample_pixel(const t_scene *scene, const t_camera *restrict cam, const uint32_t x, const uint32_t y)
{
	t_ray ray;
	t_v3 color;
	int32_t sample;
	t_v3 incoming_light;
	uint32_t seed;

	seed = (y * cam->image_width + x) + g_accummulated_frames * 792858;
	incoming_light = v3(0, 0, 0);
	ray.origin = cam->camera_center;
	sample = 0;
	while (sample < cam->samples_per_pixel)
	{
		if (cam->defocus_angle > 0)
			ray.origin = defocus_disk_sample(cam, &seed);
		ray.direction = get_ray_direction(cam, ray, (t_cord){x, y}, &seed);
		color = trace(ray, scene, &seed);
		incoming_light = V3_ADD(incoming_light, color);
		++sample;
	}
	color = f32_mul_v3(cam->pixel_sample_scale, incoming_light);
	return (color);
}

static inline
t_v3 rgb_u8_to_float(const uint8_t r, const uint8_t g, const uint8_t b)
{
	t_v3 result;

	result.r = (float)r / 255.0f;
	result.g = (float)g / 255.0f;
	result.b = (float)b / 255.0f;
	return (result);
}

static inline
t_v3 rgb_u32_to_float(uint32_t c)
{
	t_v3 result;

	result.b = (float)((c >> 16) & 0xFF) / 255.0f;
	result.g = (float)((c >> 8) & 0xFF) / 255.0f;
	result.r = (float)((c >> 0) & 0xFF) / 255.0f;
	return (result);
}

static inline
t_v3 accumulate(const t_v3 old_color, const t_v3 new_color)
{
	const float weight = 1.0 / ((g_accummulated_frames) + 1); // @TODO screw around with the weight
	t_v3 accumulated_average;

	accumulated_average.r = old_color.r * (1 - weight) + new_color.r * weight;
	accumulated_average.g = old_color.g * (1 - weight) + new_color.g * weight;
	accumulated_average.b = old_color.b * (1 - weight) + new_color.b * weight;
	return (accumulated_average);
}

void render(const t_scene *scene, const t_camera *restrict cam, uint32_t *restrict out)
{
	t_v3 color;

	uint32_t y = 0;
	while (y < cam->image_height)
	{
		uint32_t x = 0;
		while (x < cam->image_width)
		{
			color = sample_pixel(scene, cam, x, y);
			// color = get_ray_direction(&cam, x, y, x + y * cam.image_height);
			// if (g_accummulated_frames != 0)
			color = accumulate(rgb_u32_to_float(*out), color);
			*out++ = rgb_pack4x8(v3_clamp(color)); // maybe dont need clamp or do it in color correction
			++x;
		}
		++y;
	}
	// return (0);
}

void per_frame(void * param)
{
	static t_camera frame_cam = {};
	t_minirt *minirt;
	mlx_t *mlx;

	minirt = (t_minirt *)param;
	mlx = minirt->mlx;
	if (minirt->image->width != minirt->mlx->width || minirt->image->height != minirt->mlx->height)
	{
		mlx_resize_image(minirt->image, minirt->mlx->width, minirt->mlx->height);
		g_recalculate_cam = true;
	}
	if (g_recalculate_cam == true)
	{
		init_camera_for_frame(minirt, &minirt->scene.camera);
		g_recalculate_cam = false;
		g_accummulated_frames = 0;
		frame_cam = minirt->scene.camera;
	}
	// if (g_accummulated_frames < ACCUMULATE_MAX)
	// {
	render(&minirt->scene, &frame_cam, (uint32_t *)minirt->image->pixels);
	++g_accummulated_frames;
	// }
	// printf("hit: %i\tmiss: %i\n", hit, miss);

	printf("delta: %f\n", minirt->mlx->delta_time);
}
