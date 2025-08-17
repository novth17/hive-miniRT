# include "../inc/mini_rt.h"
#include <stdbool.h>
#include <stdint.h>
#include "../inc/image_to_file.h"


static uint32_t g_accummulated_frames = 0;

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
	rec.normal = noz(v3_div_f32(V3_SUB(rec.position, sp.center), sp.radius));
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
		return (-1.0f);
	sqrtd = square_root(discriminant);
	root = (v.H - sqrtd) / v.A;
	if (root <= MIN_HIT_DIST || root >= MAX_HIT_DIST)
	{
		root = (v.H + sqrtd) / v.A;
		if (root <= MIN_HIT_DIST || root >= MAX_HIT_DIST)
			return (-1.0f);
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
	return (current_hit_distance);
}

static inline
bool shadow_hit(const t_scene *scene, const t_ray ray, const float light_distance)
{
	uint32_t i;
	float hit_distance;

	// im probably calculating something wrong
	// the shadow rays are causing point lights to not work inside room_test.rt
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
		if(hit_distance > MIN_HIT_DIST && hit_distance < light_distance)// sphere_hit(&temp_rec, scene->spheres[i], ray))
			return (true);
	}
	i = -1;
	while (++i < scene->cyls_count)
	{
		hit_distance = cyl_hit(scene->cyls[i], ray);
		if(hit_distance > MIN_HIT_DIST && hit_distance < MAX_HIT_DIST)// sphere_hit(&temp_rec, scene->spheres[i], ray))
			return (true);
	}
	return (false);
}

static inline
t_v3 point_light_color(const t_scene *restrict scene, const t_hit *restrict rec, t_v3 light_direction, float dist)
{
	// look at https://en.wikipedia.org/wiki/Blinn%E2%80%93Phong_reflection_model to make this better
	// const t_color ambient_light = f32_mul_v3(scene->ambient.ratio, scene->ambient.color);
	const t_v3 point_light_color = f32_mul_v3(scene->light.bright_ratio, scene->light.color);
	float light_angle;
	t_v3 color;

	dist = dist / scene->light_dist_mult;
	// light_direction = f32_mul_v3(1.0f / dist, light_direction);
	light_angle = smoothstep(dot(rec->normal, light_direction), 0.0f, 2.0f);
	color = f32_mul_v3(light_angle, point_light_color);
	color = f32_mul_v3(1.0f / (dist * dist), color);
	// color = V3_ADD(ambient_light, color);
	// color = v3_mul_v3(rec->color, color);
	return (v3_clamp(color));
}

static inline
t_v3 check_point_light(const t_scene *restrict scene, const t_hit *restrict rec)
{
	const t_v3 light_vector = V3_SUB(scene->light.origin, rec->position);
	const float light_distance = length(light_vector);
	t_ray shadow_ray;

	shadow_ray.origin = rec->position;
	shadow_ray.direction = f32_mul_v3(1.0f / light_distance, light_vector);
	if (shadow_hit(scene, shadow_ray, light_distance) == false)
	{
		return(point_light_color(scene, rec, shadow_ray.direction, light_distance));
	}
	return (v3(0, 0, 0));
}


static inline
t_hit find_closest_ray_intesection(const t_ray ray, const t_scene * restrict scene)
{
	// t_sphere point_light_sphere = {.material.color = v3(1, 1, 1), .center = scene->light.origin, .radius = 0.05f}; // debugging
	// point_light_sphere.material.emitter = 0.0f;
	// point_light_sphere.material.diffuse = 0.0f;
	// point_light_sphere.material.specular_probability = 0.0f;
	t_hit hit_record;

	hit_record = (t_hit){};
	hit_record.distance = MAX_HIT_DIST;
	check_planes(&hit_record, scene->pls, scene->pl_count, ray);
	check_spheres(&hit_record, scene->spheres, scene->spheres_count, ray);
	// check_spheres(&hit_record, &point_light_sphere, 1, ray);
	check_cyl(&hit_record, scene->cyls, scene->cyls_count, ray);

	return (hit_record);
}

// Return true if the vector is close to zero in all dimensions.
static
bool near_zero(const t_v3 a) {
    const float s = 1e-8f;
    return ((fabsf(a.x) < s) && (fabsf(a.y) < s) && (fabsf(a.z) < s));
}


// not normalizing the random bounce direction and the final ray direction
// causes the image to be noticably brighter
// it is also quite a bit faster to render
// the light seems to not spread properly
static inline
t_ray calculate_next_ray(const t_hit *restrict rec, t_ray ray, bool is_specular_bounce, uint32_t *seed)
{
	// const t_v3 scatter = random_direction_in_hemisphere(rec->normal, seed); // do we need to normalize?
	// const t_v3 random_bounce = v3_mul_f32(scatter, !near_zero(scatter));


	// const t_v3 scatter = random_direction_in_hemisphere(rec->normal, seed);
	// const t_v3 random_bounce = noz(random_direction_in_hemisphere(rec->normal, seed)); // do we need to normalize?

	const t_v3 scatter = in_unit_sphere(seed);
	const t_v3 random_bounce = normalize(V3_ADD(rec->normal, v3_mul_f32(scatter, !near_zero(scatter))));
	t_v3 pure_bounce;

	// inner product (or dot product) gives us the angle difference between
	// the normal at the hit point and the direction of the ray
	// that angle times 2 is the direction we want the mirror reflection to go
	pure_bounce = f32_mul_v3(2.0f*dot(ray.direction, rec->normal), rec->normal);
	pure_bounce = v3_sub_v3(ray.direction, pure_bounce);

	// ray.origin = V3_SUB(rec->position, v3_mul_f32(ray.direction, 1e-4f)); // look to see if this value is good or not
	ray.origin = rec->position;
	// ray.origin = V3_ADD(rec->position, v3_mul_f32(rec->normal, 1e-4f));
	ray.direction = noz(v3_lerp(random_bounce, rec->mat.diffuse * is_specular_bounce, pure_bounce)); // do we need to normalize?






	return (ray);
}

static inline
t_v4 trace(t_ray ray, const t_scene * restrict scene, const uint32_t max_bounce, uint32_t *seed) // change to all objects or scene;
{
	t_v3 ambient = f32_mul_v3(scene->ambient.ratio, scene->ambient.color);
	// t_v3 point_light_color = f32_mul_v3(scene->light.bright_ratio * scene->light_strength_mult, scene->light.color);
	// t_sphere point_light_sphere = {.color = v3(20, 20, 20), .center = scene->light.origin, .radius = 0.05f};
	uint32_t i;
	t_color total_incoming_light;


	t_hit rec;
	t_color ray_color;
	t_color prev_color;
	ray_color = v3(1, 1, 1);
	prev_color = ray_color;
	i = 0;
	total_incoming_light = v3(0, 0, 0);
	bool hit_once = false;
	bool prev_specular = true;
	while (i <= max_bounce)
	{
		rec = find_closest_ray_intesection(ray, scene);
		if (rec.did_hit)
		{
			// if we hit something calculate the light contribution of that point into the total light of the ray

			rec.mat.specular_color = rec.mat.color; // here for now
			hit_once = true;
			rec.position = V3_ADD(rec.position, v3_mul_f32(rec.normal, 1e-4f));
			// rec.position = V3_SUB(rec.position, v3_mul_f32(ray.direction, 1e-4f));
			const bool is_specular_bounce = rec.mat.specular_probability >= random_float(seed);
			ray_color = v3_mul_v3(ray_color, v3_lerp(rec.mat.color, is_specular_bounce, rec.mat.specular_color));
			if (scene->use_point_light)
			{
				total_incoming_light = V3_ADD(total_incoming_light, v3_mul_v3(check_point_light(scene, &rec), ray_color));
			}
			t_color emmitted_light = v3_mul_f32(rec.mat.color, rec.mat.emitter);
			total_incoming_light = V3_ADD(total_incoming_light, v3_mul_v3(emmitted_light, ray_color));

			if (i > 0 && !prev_specular)
			{
				float p = fmax(ray_color.r, fmax(ray_color.g, ray_color.b));
				if (random_float(seed) >= p)
				{
					ray_color = prev_color;
					break;
				}
			}
			prev_specular = is_specular_bounce;
			prev_color = ray_color;
			ray = calculate_next_ray(&rec, ray, is_specular_bounce, seed);
			++i;
		}
		else
		{

			break ;
		}
	}
	if (hit_once)
	{
		total_incoming_light = V3_ADD(total_incoming_light, v3_mul_v3(ambient, ray_color));
		return ((t_v4){.rgb = v3_clamp(total_incoming_light), .a = 1.0f});
	}

	return (v4(0, 0, 0, 0));
}


// for calculating the defocus blur effect
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
	const t_v2 offset = sample_square_stratified(strati.x, strati.y, cam->recip_sqrt_spp, seed);
	// const t_v2 offset = {};
	const t_v3 x_delta = f32_mul_v3(cord.x + offset.x, cam->pixel_delta_u);
	const t_v3 y_delta = f32_mul_v3(cord.y + offset.y, cam->pixel_delta_v);
	const t_v3 pixel_sample = V3_ADD(cam->pixel00_loc, V3_ADD(x_delta, y_delta));
	t_ray ray;

	ray.origin = cam->camera_center;
	if (cam->defocus_angle > 0.0f)
		ray.origin = defocus_disk_sample(cam, seed);
	ray.direction = normalize(V3_SUB(pixel_sample, ray.origin));
	return (ray);
}

t_v4 sample_pixel(const t_scene *scene, const t_camera *restrict cam, const t_cord original_cord, uint32_t seed)
{
	t_ray ray;
	t_v4 color;
	t_v4 incoming_light;
	int y_s;
	int x_s;

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
t_v4 accumulate(const t_v4 old_color, const t_v4 new_color)
{
	const float weight = 1.0 / (g_accummulated_frames + 1); // @TODO screw around with the weight
	t_v4 accumulated_average;

	accumulated_average.r = old_color.r * (1 - weight) + new_color.r * weight;
	accumulated_average.g = old_color.g * (1 - weight) + new_color.g * weight;
	accumulated_average.b = old_color.b * (1 - weight) + new_color.b * weight;
	accumulated_average.a = old_color.a * (1 - weight) + new_color.a * weight;
	return (accumulated_average);
}

void render(const t_scene *scene, const t_camera *restrict cam, uint32_t *restrict out)
{
	t_v4 color;
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

			color = accumulate(exact_unpack(*out), color);
			*out++ = exact_pack(color);

			// color = accumulate(rgb_u32_to_float(*out), color);
			// *out++ = rgb_pack4x8(v3_clamp(color)); // maybe dont need clamp or do it in color correction


			++x;
		}
		++y;
	}
	// return (0);
}

static
void set_title(t_minirt *minirt)
{
	static char title_buf[100] = "MiniRay --";
	int status;
	t_string title;

	title.buf = title_buf;
	title.size = sizeof(title_buf);
	title.len = sizeof("MiniRay --") - 1;
	status = 0;
	status = cat_cstring_to_string(&title, " frame time: ");
	status = cat_uint_to_str(&title, round(minirt->mlx->delta_time * 1000));
	status = cat_cstring_to_string(&title, " SPP: ");
	status = cat_uint_to_str(&title, minirt->scene.camera.samples_per_pixel);
	status = cat_cstring_to_string(&title, " max bounces: ");
	status = cat_uint_to_str(&title, minirt->scene.camera.max_bounce);
	if (status == FAIL)
	{
		ft_dprintf(2, "Failed to create title\n"
			"buf_size <%u> len <%u> data <%s>\n", title.size, title.len, title.buf);
		mlx_set_window_title(minirt->mlx, "MiniRay -- Info not available");
		return ;
	}
	mlx_set_window_title(minirt->mlx, title.buf);
}

static
void recalculate_camera(t_minirt *minirt, t_camera *frame_cam)
{
	init_camera_for_frame(minirt, &minirt->scene.camera);
	*frame_cam = minirt->scene.camera;
	minirt->recalculate_cam = false;
	g_accummulated_frames = 0;
}


void per_frame(void * param)
{
	static t_camera frame_cam = {};
	t_minirt *minirt;
	mlx_t *mlx;

	minirt = (t_minirt *)param;
	mlx = minirt->mlx;
	set_title(minirt);
	if (minirt->write_image_to_file == true)
	{
		pixels_to_image_file(minirt->image);
		minirt->write_image_to_file = false;
	}
	if (minirt->image->width != (uint)mlx->width || minirt->image->height != (uint)mlx->height)
	{
		if (mlx_resize_image(minirt->image, mlx->width, mlx->height) == false)
			ft_putstr_fd("miniRT: Failed to resize image\n", 2);
		if (mlx_resize_image(minirt->background, mlx->width, mlx->height) == false)
			ft_putstr_fd("miniRT: Failed to resize background\n", 2);
		recalculate_camera(minirt, &frame_cam);
		draw_background(minirt);
	}
	if (minirt->recalculate_cam == true)
	{
		recalculate_camera(minirt, &frame_cam);
	}
	render(&minirt->scene, &frame_cam, (uint32_t *)minirt->image->pixels);
	++g_accummulated_frames;
}
