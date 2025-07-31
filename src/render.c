# include "../inc/mini_rt.h"
#include <stdbool.h>
#include <stdint.h>
#include "../inc/image_to_file.h"


extern bool g_recalculate_cam;
extern uint32_t g_accummulated_frames;

#if 1
	#define STRATI
#endif

#define A x
#define H y
#define C z

inline
void set_face_normal(t_hit * restrict rec, const t_ray *restrict ray)
{
	rec->front_face = dot(ray->direction, rec->normal) < 0;
	if (false == rec->front_face)
		rec->normal = neg(rec->normal);
}


static inline
t_hit create_sphere_hit_record(const t_ray ray, const t_sphere sp, const float root)
{
	t_hit rec;

	rec.did_hit = true;
	rec.distance = root;
	rec.position = at(ray, rec.distance);
	rec.mat = sp.material;
	rec.normal = v3_div_f32(V3_SUB(rec.position, sp.center), sp.radius);
	set_face_normal(&rec, &ray);
	return (rec);
}

static inline
float sphere_hit(const t_sphere sp, const t_ray ray)
{
	const t_v3 oc = v3_sub_v3(sp.center, ray.origin);
	const t_v3 v = {
			.A = dot(ray.direction, ray.direction), 	// const float a = dot(ray.direction, ray.direction);
			.H = dot(ray.direction, oc), 				// const float h = dot(ray.direction, oc);
			.C = dot(oc, oc) - sp.radius*sp.radius}; 	// const float c = dot(oc, oc) - sp.radius*sp.radius;
	const float discriminant = v.H*v.H - v.A*v.C;
	float sqrtd;
	float root;

	if (discriminant < 0)
		return (FLT_MAX);
	sqrtd = square_root(discriminant);
	root = (v.H - sqrtd) / v.A;
	if (root <= MIN_HIT_DIST || root >= MAX_HIT_DIST)
	{
		root = (v.H + sqrtd) / v.A;
		if (root <= MIN_HIT_DIST || root >= MAX_HIT_DIST)
			return (FLT_MAX);
	}
	return (root);
}

static inline
float check_spheres(t_hit *restrict rec, const t_sphere *spheres, const uint32_t count, const t_ray ray)
{
	// t_hit temp_rec;
	uint32_t i;

	// temp_rec = (t_hit){};
	// temp_rec.distance = MAX_HIT_DIST;
	float hit_distance;
	float current_hit_distance = rec->distance;
	i = 0;
	while (i < count)
	{
		hit_distance = sphere_hit(spheres[i], ray);
		if(hit_distance > MIN_HIT_DIST && hit_distance < current_hit_distance)
		{
			*rec = create_sphere_hit_record(ray, spheres[i], hit_distance); // @QUESTION when to use set_face_normal - here or later?
			current_hit_distance = hit_distance;
			// *rec = temp_rec;
		}
		++i;
	}
	return (hit_distance);
}

static inline
bool shadow_hit(const t_scene *scene, const t_ray ray) // change to all objects or scene;
{
	uint32_t i;
	float hit_distance;

	// i = -1;
	// while (++i < scene->pl_count)
	// {
	// 	if (plane_shadow_hit(scene->pls[i], ray))
	// 		return (true);
	// 	++i;
	// }
	i = -1;
	while (++i < scene->spheres_count)
	{
		hit_distance = sphere_hit(scene->spheres[i], ray);
		if(hit_distance > MIN_HIT_DIST && hit_distance < MAX_HIT_DIST)// sphere_hit(&temp_rec, scene->spheres[i], ray))
			return (true);
	}
	// i = -1;
	// while (++i < scene->cyls_count)
	// {
	// 	if (plane_shadow_hit(scene->cyls[i], ray))
	// 	{
	// 		return (true);
	// 	}
	// }
	return (false);
}







static inline
float smoothstep(const float edge0, const float edge1, float x)
{
	x = clamp((x - edge0) / (edge1 - edge0), edge0, edge1);
	return (x * x * (3.0f - 2.0f * x));
}

// static inline
// t_v3 v3_smoothstep(const t_v3 value)
// {
// 	t_v3 result;

// 	result.x = smoothstep(value.x, 0.0f, 1.0f);
// 	result.y = smoothstep(value.x, 0.0f, 1.0f);
// 	result.z = smoothstep(value.x, 0.0f, 1.0f);
// 	return (result);
// }

static inline
t_v3 point_light_color(const t_scene *restrict scene, const t_hit *restrict rec, t_v3 light_direction)
{
	// const t_color ambient_light = f32_mul_v3(scene->ambient.ratio, scene->ambient.color);
	const t_v3 point_light_color = f32_mul_v3(scene->light.bright_ratio * scene->light_strength_mult, scene->light.color);
	const float dist = length(light_direction);
	float light_angle;
	t_v3 color;

	light_direction = f32_mul_v3(1.0f / dist, light_direction);
	light_angle = smoothstep(dot(rec->normal, light_direction), 0.0f, 2.0f);
	color = f32_mul_v3(light_angle, point_light_color);
	color = f32_mul_v3(1.0f / (dist * dist), color);
	// color = V3_ADD(ambient_light, color);
	// color = v3_mul_v3(rec->color, color);
	return (color);
}

static inline
t_v3 check_point_light(const t_scene *restrict scene, const t_hit *restrict rec)
{
	t_v3 l;
	t_ray shadow_ray;
	t_v3 light_color;

	l = V3_SUB(scene->light.origin, rec->position);
	shadow_ray.origin = V3_ADD(rec->position, v3_mul_f32(rec->normal, 1e-4));
	shadow_ray.direction = l;
	light_color = v3(0, 0, 0);
	if (shadow_hit(scene, shadow_ray) == false)
	{
		light_color = point_light_color(scene, rec, l);
	}
	return (light_color);
}


static inline
t_hit find_closest_ray_intesection(const t_ray ray, const t_scene * restrict scene)
{
	t_sphere point_light_sphere = {.material.color = v3(20, 20, 20), .center = scene->light.origin, .radius = 0.05f}; // debugging
	point_light_sphere.material.emitter = 0.0f;
	point_light_sphere.material.diffuse = 0.0f;
	point_light_sphere.material.specular_probability = 0.0f;
	t_hit hit_record;

	hit_record = (t_hit){};
	hit_record.distance = MAX_HIT_DIST;
	check_planes(&hit_record, scene->pls, scene->pl_count, ray);
	check_spheres(&hit_record, scene->spheres, scene->spheres_count, ray);
	check_spheres(&hit_record, &point_light_sphere, 1, ray);

	return (hit_record);
}

static inline
float random_float_normal_dist(uint32_t *seed)
{
	const float theta = 2 * M_PI + random_float(seed);
	const float rho = square_root(-2 * log(random_float(seed)));
	return (rho * cos(theta));
}

static inline
t_v3 random_direction_normal_dist(uint32_t *seed)
{
	const float x = random_float_normal_dist(seed);
	const float y = random_float_normal_dist(seed);
	const float z = random_float_normal_dist(seed);

	return (normalize(v3(x, y, z)));
}

t_v3 random_direction_in_hemisphere(const t_v3 normal, uint32_t *rng_seed)
{
	const t_v3 dir = random_direction_normal_dist(rng_seed);
	return (v3_mul_f32(dir, sign(dot(normal, dir))));
}

// Return true if the vector is close to zero in all dimensions.
static
bool near_zero(const t_v3 a) {
    const float s = 1e-8;
    return ((fabsf(a.x) < s) && (fabsf(a.y) < s) && (fabsf(a.z) < s));
}


// not normalizing the random bounce direction and the final ray direction
// causes the image to be noticably brighter
// it is also quite a bit faster to render
// the light seems to not spread properly
static inline
t_ray calculate_next_ray(const t_hit *restrict rec, t_ray ray, bool is_specular_bounce, uint32_t *seed)
{
	const t_v3 scatter = in_unit_sphere(seed);
	const t_v3 random_bounce = noz(V3_ADD(rec->normal, v3_mul_f32(scatter, !near_zero(scatter))));
	// const t_v3 random_bounce = noz(random_direction_in_hemisphere(rec->normal, seed)); // do we need to normalize?
	t_v3 pure_bounce;

	// inner product (or dot product) gives us the angle difference between
	// the normal at the hit point and the direction of the ray
	// that angle times 2 is the direction we want the mirror reflection to go
	pure_bounce = f32_mul_v3(2.0f*dot(ray.direction, rec->normal), rec->normal);
	pure_bounce = v3_sub_v3(ray.direction, pure_bounce);

	ray.origin = rec->position;
	ray.direction = noz(v3_lerp(random_bounce, rec->mat.diffuse * is_specular_bounce, pure_bounce)); // do we need to normalize?
	return (ray);
}

static inline
t_v3 trace(t_ray ray, const t_scene * restrict scene, const int32_t max_bounce, uint32_t *seed) // change to all objects or scene;
{
	static const t_color specular_color = {.r = 1.0f, .g = 1.0f, .b = 1.0f};
	t_v3 ambient = f32_mul_v3(scene->ambient.ratio, scene->ambient.color);
	// t_v3 point_light_color = f32_mul_v3(scene->light.bright_ratio * scene->light_strength_mult, scene->light.color);
	// t_sphere point_light_sphere = {.color = v3(20, 20, 20), .center = scene->light.origin, .radius = 0.05f};
	int32_t i;
	t_color total_incoming_light;


	t_hit rec;
	t_color ray_color;
	ray_color = v3(1, 1, 1);
	i = 0;
	total_incoming_light = v3(0, 0, 0);
	bool hit_once = false;
	while (i <= max_bounce)
	{
		rec = find_closest_ray_intesection(ray, scene);
		if (rec.did_hit)
		{
			rec.mat.specular_color = rec.mat.color; // here for now
			hit_once = true;
			// there is a bug here relating to ambient light and how that affects the color of an object
			// it seems to cause the object to take on the color of the ambient light even if it should not
			// might not be a bug technically just related to the fact that the specular bounce is always the same
			if (scene->use_point_light)
			{
				total_incoming_light = V3_ADD(total_incoming_light, v3_mul_v3(check_point_light(scene, &rec), ray_color));
			}
			t_color emmitted_light = v3_mul_f32(rec.mat.color, rec.mat.emitter);
			total_incoming_light = V3_ADD(total_incoming_light, v3_mul_v3(emmitted_light, ray_color));
			const bool is_specular_bounce = rec.mat.specular_probability >= random_float(seed);
			ray_color = v3_mul_v3(ray_color, v3_lerp(rec.mat.color, is_specular_bounce, rec.mat.specular_color));
			// color = V3_ADD(ambient, color);
			// color = v3_mul_v3(rec.color, color);

			ray = calculate_next_ray(&rec, ray, is_specular_bounce, seed);
			// ray.direction = rec.normal;
			++i;
		}
		else
		{
			// total_incoming_light = V3_ADD(total_incoming_light, v3_mul_v3(ambient, ray_color));
			// color = V3_ADD(ambient, color);
			// color = v3_mul_v3(rec.color, color);
			break ;
		}
	}
	if (hit_once)
	{
		total_incoming_light = V3_ADD(total_incoming_light, v3_mul_v3(ambient, ray_color));
		return (total_incoming_light);
	}

	// not hit = background color
	// t_v3 unit_direction = unit_vector(ray.direction);
	// float a = 0.5 * (unit_direction.y + 1.0f);

	// t_v3 result;
	// result = f32_mul_v3(1.0 - a, v3(1.0, 1.0, 1.0));
	// result = v3_add_v3(result, f32_mul_v3(a, v3(0.5, 0.7, 1.0)));
	// // incoming_ligth = result; // for now
	// return (result);
	return (v3(0, 0, 0));
}

static inline
t_v3 v4_to_v3(t_v4 a)
{
	return (v3(a.x, a.y, a.z));
}

static inline
t_v4 quaternion_thingy_dont_know(t_v4 target)
{
	t_v3 target_v3 = v4_to_v3(target);
	target_v3 =  unit_vector(v3_div_f32(target_v3, target.w));
	target = (t_v4){.xyz = target_v3, .w = 0};
	return (target);
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
t_ray get_ray(const t_camera *cam, t_cord cord, t_cord strati, uint32_t *seed)
{
#ifdef STRATI
	const t_v2 offset = sample_square_stratified(strati.x, strati.y, cam->recip_sqrt_spp, seed);
#else
	const t_v2 offset = sample_square(seed);
#endif
	const t_v3 x_delta = f32_mul_v3(cord.x + offset.x, cam->pixel_delta_u);
	const t_v3 y_delta = f32_mul_v3(cord.y + offset.y, cam->pixel_delta_v);
	const t_v3 pixel_sample = V3_ADD(cam->pixel00_loc, V3_ADD(x_delta, y_delta));
	t_ray ray;

	ray.origin = cam->camera_center;
	if (cam->defocus_angle > 0)
		ray.origin = defocus_disk_sample(cam, seed);
	ray.direction = V3_SUB(pixel_sample, ray.origin);
	return (ray);
}

// static inline
// t_v3 get_ray_direction(const t_camera *cam, const t_ray ray, t_cord cord, uint32_t *seed)
// {
// 	static uint64_t num = 0;
// 	const t_v2 offset = sample_square_stratified(cord.x, cord.y, cam->recip_sqrt_spp, seed);
// 	// printf("offset x <%f> offset y <%f>\n", offset.x, offset.y);
// 	// t_v3 offset = {0, 0, 0};
// 	const t_v3 x_delta = f32_mul_v3(cord.x + offset.x, cam->pixel_delta_u);
// 	const t_v3 y_delta = f32_mul_v3(cord.y + offset.y, cam->pixel_delta_v);
// 	const t_v3 pixel_sample = V3_ADD(cam->pixel00_loc, V3_ADD(x_delta, y_delta));

// 	cord.x = (float)(cord.x + offset.x) / (float)cam->image_width;
// 	cord.y = (float)(cord.y + offset.y) / (float)cam->image_height;

// 	cord.x = cord.x * 2.0f - 1.0f;
// 	cord.y = cord.y * 2.0f - 1.0f;
// 	// t_v4 target = mat_mul_v4(cam->inverse_projection, v4(cord.x, cord.y, 1.0f, 1.0f));
// 	// t_v3 direction = v4_to_v3(mat_mul_v4(cam->inverse_view, quaternion_thingy_dont_know(target)));
// 	t_v3 direction = V3_SUB(pixel_sample, ray.origin);
// 	// t_v4 temp = (t_v4){.xyz = direction, 0};

// 	// temp = mat_mul_v4(cam->inverse_projection, temp);
// 	// temp = mat_mul_v4(cam->inverse_view, temp);
// 	// direction = v4_to_v3(temp);
// 	// t_v3 direction = V3_SUB(v4_to_v3(target), ray.origin);
// 	// if (num++ % 4096 == 0)
// 	// 	printf("%f %f %f\n", direction.x, direction.y, direction.z);
// 	return (direction);
// }

t_v3 sample_pixel(const t_scene *scene, const t_camera *restrict cam, const t_cord cord, uint32_t seed)
{
	t_ray ray;
	t_v3 color;
	t_v3 incoming_light;

	incoming_light = v3(0, 0, 0);
	// ray.origin = cam->camera_center;


#ifdef STRATI
	int y_s;
	int x_s;

	y_s = 0;
	while (y_s < cam->sqrt_spp)
	{
		x_s = 0;
		while (x_s < cam->sqrt_spp)
		{

			ray = get_ray(cam, cord, (t_cord){x_s, y_s}, &seed);
			color = trace(ray, scene, cam->max_bounce, &seed);
			incoming_light = V3_ADD(incoming_light, color);
			++x_s;
		}
		++y_s;
	}
	color = f32_mul_v3(cam->pixel_sample_scale_strati, incoming_light);
#else
	int32_t sample;

	sample = 0;
	while (sample < cam->samples_per_pixel)
	{
		ray = get_ray(cam, cord, cord, &seed);
		color = trace(ray, scene, cam->max_bounce, &seed);
		incoming_light = V3_ADD(incoming_light, color);
		++sample;
	}
	color = f32_mul_v3(cam->pixel_sample_scale, incoming_light);
#endif
	return (color);
}

// static inline
// t_v3 rgb_u8_to_float(const uint8_t r, const uint8_t g, const uint8_t b)
// {
// 	t_v3 result;

// 	result.r = (float)r / 255.0f;
// 	result.g = (float)g / 255.0f;
// 	result.b = (float)b / 255.0f;
// 	return (result);
// }

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
	int32_t x;
	int32_t y;
	uint32_t rng_seed;

	y = 0;
	while (y < cam->image_height)
	{
		x = 0;
		while (x < cam->image_width)
		{
			rng_seed = (y * cam->image_width + x) + g_accummulated_frames * 792858;
			color = sample_pixel(scene, cam, (t_cord){x, y}, rng_seed);
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
	if (minirt->write_image_to_file == true)
	{
		pixels_to_image_file(minirt->image);
		minirt->write_image_to_file = false;
	}
	if (minirt->image->width != (uint)mlx->width || minirt->image->height !=  (uint)mlx->height)
	{
		mlx_resize_image(minirt->image, mlx->width, mlx->height);
		g_recalculate_cam = true;
	}
	if (g_recalculate_cam == true)
	{
		init_camera_for_frame(minirt, &minirt->scene.camera);
		g_recalculate_cam = false;
		g_accummulated_frames = 0;
		frame_cam = minirt->scene.camera;
	}
	render(&minirt->scene, &frame_cam, (uint32_t *)minirt->image->pixels);
	++g_accummulated_frames;
	printf("delta: %f\n", minirt->mlx->delta_time);
}
