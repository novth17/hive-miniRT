/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 22:28:07 by hiennguy          #+#    #+#             */
/*   Updated: 2025/07/24 03:41:37 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/mini_rt.h"
#include "../inc/rt_math.h"
#include "../test_stuff/MLX42.h"

static bool recalculate_cam = true;


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
// #include "camera.c"

#include <float.h>
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

// static inline
// t_ray init_ray(t_camera *cam, int32_t x, int32_t y);

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

// static inline
// bool sphere_hit(t_sphere sp, t_ray ray)
// {
// 	const t_v3 oc = v3_sub_v3(sp.position, ray.origin);

// 	const float a = dot(ray.direction, ray.direction);
// 	const float b = -2.0f * dot(ray.direction, oc);
// 	const float c = dot(oc, oc) - sp.radius*sp.radius;
// 	const float discrimanant = b*b - 4*a*c;
// 	if (discrimanant >= 0)
// 	{
// 		return true;
// 	}
// 	else
// 		return false;
// }

typedef struct s_hit_record
{
	t_v3 position; // p
	t_v3 normal;
	float distance; // t
	bool front_face; // maybe not needed;
	t_v3 color;
} t_hit;

#define MIN_HIT_DIST 0.001f
#define MAX_HIT_DIST FLT_MAX // for now

static inline // @TODO move this
t_v3 at(const t_ray r, const float t)
{
	t_v3 result;

	result = V3_ADD(r.origin, f32_mul_v3(t, r.direction));
	return (result);
}

#define A x
#define H y
#define C z

static inline
bool sphere_hit(t_hit *rec, const t_sphere sp, const t_ray ray)
{
	const t_v3 oc = v3_sub_v3(sp.position, ray.origin);
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
	rec->distance = root;
	rec->position = at(ray, rec->distance);
	rec->normal = v3_div_f32(V3_SUB(rec->position, sp.position), sp.radius);
	rec->color = sp.color;
	return (true);
}

inline
t_v3 neg(t_v3 a)
{
	t_v3 result;

	result.x = -a.x;
	result.y = -a.y;
	result.z = -a.z;
	return (result);
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

static inline
bool check_spheres(t_hit *rec, const t_spheres *spheres, const t_ray ray)
{
	t_hit temp_rec;
	uint32_t i;

	temp_rec.distance = MAX_HIT_DIST;
	i = 0;
	while (i < spheres->count)
	{
		if(sphere_hit(&temp_rec, spheres->arr[i], ray))
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
	return (temp_rec.distance < MAX_HIT_DIST);
}

// static uint32_t rng_state = 25021129;

// inline
// uint32_t pcg_hash(uint32_t seed)
// {
//     const uint32_t state = seed * 747796405u + 2891336453u;
//     const uint32_t word = ((state >> ((state >> 28u) + 4u)) ^ state) * 277803737u;

//     return ((word >> 22u) ^ word);
// }


/// @brief gives a random float between 0.0f and 1.0f
/// @param seed seed for the generation
/// @return random number
inline
float random_float(uint32_t *seed)
{
 	const uint32_t state = *seed * 747796405u + 2891336453u;
    const uint32_t result = ((state >> ((state >> 28u) + 4u)) ^ state) * 277803737u;

    *seed = state;
	return ((float)result / (float)UINT32_MAX);
}

// wrong and not currently used
t_v3 in_unit_sphere(uint32_t *seed)
{
	t_v3 result;

	result.x = random_float(seed) * 2.0f - 1.0f;
	result.y = random_float(seed) * 2.0f - 1.0f;
	result.z = random_float(seed) * 2.0f - 1.0f;
	result = normalize(result);
	return (result);
}

inline
float clamp(const float f, const float min, const float max)
{
	if (f < min)
		return (min);
	else if (f > max)
		return (max);
	else
		return (f);
}

inline
t_v3 sample_square(uint32_t *seed)
{
	t_v3 result;

	result.x = random_float(seed) - 0.5f;
	result.y = random_float(seed) - 0.5f;
	result.z = 0;
	return (result);
}

static inline
t_v3 get_offset(uint32_t *seed)
{
	t_v3 result = {};
	result = in_unit_sphere(seed);
	return (result);
}

// static uint32_t seed = 124124;

static inline
t_v3 get_ray_direction(const t_camera *cam, int32_t x, int32_t y, uint32_t *seed)
{
	const t_v3 offset = sample_square(seed);
	// printf("offset x <%f> offset y <%f>\n", offset.x, offset.y);
	// t_v3 offset = {0, 0, 0};
	const t_v3 x_delta = f32_mul_v3(x + offset.x, cam->pixel_delta_u);
	const t_v3 y_delta = f32_mul_v3(y + offset.y, cam->pixel_delta_v);
	const t_v3 pixel_sample = V3_ADD(cam->pixel00_loc, V3_ADD(x_delta, y_delta));

	return (V3_SUB(pixel_sample, cam->camera_center));
}

inline
float smoothstep(const float edge0, const float edge1, float x)
{
	x = clamp((x - edge0) / (edge1 - edge0), edge0, edge1);
	return (x * x * (3.0f - 2.0f * x));
}


// not used currently might never be wooooo
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
bool shadow_hit(const t_ray ray, const t_spheres *spheres) // change to all objects or scene;
{
	;
}


static inline
t_v3 trace(t_ray ray, const t_spheres *spheres) // change to all objects or scene;
{

	t_v3 ambient = f32_mul_v3(0.1, v3(1, 1, 1));
	t_v3 point_ligth_loc = v3(10, 10, 10);
	t_v3 point_light_color = f32_mul_v3(1000, v3(0.9, 1, 1));
	t_sphere point_ligth_spehre = {.color = v3(1, 1, 1), .position = point_ligth_loc, .radius = 0.05f};
	t_spheres point_ligth_visualization = {.count = 1, .arr = &point_ligth_spehre};

	// bounces mby here at some point
	t_v3 incoming_ligth;
	// t_v3 ray_colour;
	t_hit rec;

	incoming_ligth = (t_v3){};
	// ray_colour = (t_v3){1, 1, 1};
	// for (int i = 0; i < 1; ++i)
	// {

		rec = (t_hit){};
		rec.distance = MAX_HIT_DIST;
		check_spheres(&rec, spheres, ray);
		check_spheres(&rec, &point_ligth_visualization, ray); // testing
		//check_planes(&rec, planes, ray);
		//check_cylinders(&rec, spheres, ray);
		if (rec.distance < MAX_HIT_DIST)
		{
			// ray.origin = rec.position;
			// ray.direction = rec.normal;
			// if (hit_point_light())
			t_v3 color;
			// {
				t_v3 l =  V3_SUB(point_ligth_loc, rec.position);
				float dist = length(l);
				l = f32_mul_v3(1.0f / dist, l);
				// float light_angle = clamp(dot(rec.normal, l), 0.0f, FLT_MAX);
				// float light_angle = clamp(dot(rec.normal, l), 0.0f, FLT_MAX);
				float light_angle = smoothstep(dot(rec.normal, l), 0.0f, 4.0f);
				// float light_angle = dot(rec.normal, l);color
				color = f32_mul_v3(light_angle, point_light_color);
				color = f32_mul_v3(1.0f / (dist * dist), color);
				color = V3_ADD(ambient, color);
				// color = v3_mul_v3(f32_mul_v3(0.5, V3_ADD(rec.normal, v3(1, 1, 1))), color); // reflectivity of material * color calc
			// }
			// ray_colour = v3_mul_v3(ray_colour, color);
			return (color);
			// incoming_ligth = V3_ADD(incoming_ligth, ray_colour);
			// incoming_ligth = color;

				// ray_colour = v3_mul_v3(rec.color, ray_colour);
			// ray_colour = v3_mul_v3(ray_colour, rec.color);
		}
		else
		{
			t_v3 unit_direction = unit_vector(ray.direction);
			float a = 0.5 * (unit_direction.y + 1.0f);

			t_v3 result;
			result = f32_mul_v3(1.0 - a, v3(1.0, 1.0, 1.0));
			result = v3_add_v3(result, f32_mul_v3(a, v3(0.5, 0.7, 1.0)));
			// incoming_ligth = V3_ADD(incoming_ligth, v3_mul_v3(ambient, incoming_ligth));
			return (result);
			// break ;
		}
	// }
	// return (v3(.1,.1,.1));

	// t_v3 hit_color = v3_mul_v3(ambient, ray_color(ray, &spheres));
	// hit_color = V3_ADD(hit_color, light_intercetion())



	// not hit = background color
	// t_v3 unit_direction = unit_vector(ray.direction);
	// float a = 0.5 * (unit_direction.y + 1.0f);

	// t_v3 result;
	// result = f32_mul_v3(1.0 - a, v3(1.0, 1.0, 1.0));
	// result = v3_add_v3(result, f32_mul_v3(a, v3(0.5, 0.7, 1.0)));
	// incoming_ligth = result; // for now
	return (incoming_ligth);
}

void base_init_cam(t_minirt *minirt, t_camera *cam);

// uint32_t sample_pixel()
// {

// }



inline
t_v3 v3_clamp(const t_v3 a)
{
	t_v3 result;

	result.x = clamp(a.x, 0, 1);
	result.y = clamp(a.y, 0, 1);
	result.z = clamp(a.z, 0, 1);
	return (result);
}

inline
uint32_t rgb_pack4x8(t_v3 unpacked)
{
	uint32_t result;


	result = ((uint32_t)(0xFF) << 24)					|
			((uint32_t)((unpacked.b) * 255.0f) << 16)	|
			((uint32_t)((unpacked.g) * 255.0f) << 8)	|
			((uint32_t)((unpacked.r) * 255.0f) << 0);
	return (result);
}

// void trace(t_scene *scene, const t_camera * restrict cam, uint32_t *pixels)
// void trace(t_spheres *spheres, const t_camera * restrict cam, uint32_t *pixels)
// {
// 	uint32_t y;
// 	uint32_t x;
// 	uint32_t sample;
// 	t_v3	color;

// 	y = 0;
// 	while (y < cam->image_height)
// 	{
// 		x = 0;
// 		while (x < cam->image_width)
// 		{
// 			sample = 0;
// 			while (sample < cam->samples_per_pixel)
// 			{
// 				color = ray_color(ray, const t_spheres *spheres)
// 				++sample;
// 			}
// 			*pixels++ = rgb_pack4x8(color);
// 			// mlx_put_pixel(minirt->image, x, y, uint32_t color)
// 			++x;
// 		}
// 		++y;
// 	}
// }


void key_hook(struct mlx_key_data data, void * param)
{
	const float speed = 0.5f;
	t_minirt *minirt;
	t_v3 direction;

	minirt = (t_minirt *)param;
	// printf("keyhookend at %f\n", minirt->mlx->delta_time);
	// if (mlx_is_mouse_down(minirt->mlx, MLX_MOUSE_BUTTON_RIGHT))
	if (data.action != MLX_RELEASE)
	{
		recalculate_cam = true;
		if (data.key == MLX_KEY_W)
		{
			direction = f32_mul_v3(minirt->mlx->delta_time * speed, minirt->base_cam.lookat);
			minirt->base_cam.lookfrom = V3_ADD(minirt->base_cam.lookfrom, direction);
		}
		else if (data.key == MLX_KEY_S)
		{
			direction = f32_mul_v3(minirt->mlx->delta_time * speed, minirt->base_cam.lookat);
			minirt->base_cam.lookfrom = V3_SUB(minirt->base_cam.lookfrom, direction);
		}
	}
	// }
}

// mlx_key_hook(mlx_t *mlx, mlx_keyfunc func, void *param)

static uint32_t g_frame_num = 1;

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
t_v3 accumulate(t_v3 old_color, t_v3 new_color)
{
	const float weight = (1.0 / g_frame_num + 1);
	t_v3 accumulated_average;

	accumulated_average.r = old_color.r * (1 - weight) + new_color.r * weight;
	accumulated_average.g = old_color.g * (1 - weight) + new_color.g * weight;
	accumulated_average.b = old_color.b * (1 - weight) + new_color.b * weight;
	return (accumulated_average);
}


static
void render(t_scene *scene, const t_camera *restrict cam, uint32_t *restrict out)
{

	// t_v3 ambient = f32_mul_v3(scene->ambient.ratio, scene->ambient.color);
	// t_v3 ambient = f32_mul_v3(0.1, v3(1, 1, 1));
	// t_v3 point_ligth_loc = v3(10, 10, -10);
	// t_v3 point_light_color = f32_mul_v3(0.1, v3(1, 1, 1));

	static const int count = 3;

	t_spheres spheres;
	t_sphere arr[count];
	spheres.count = count;
	spheres.arr = arr;
	arr[0].position = v3(0, 0, -1);
	arr[0].radius = 0.5f;
	arr[0].color = v3(1, 0, 0);
	arr[1].position = v3(0, -55, 0);
	arr[1].radius = 54.0f;
	arr[1].color = v3(1, 1, 1);

	arr[2].position = v3(1, 1, 0);
	arr[2].radius = 0.3f;
	arr[2].color = v3(0, 1, 1);


	t_ray ray;
	ray.origin = cam->camera_center;

	const float pixel_sample_scale = 1.0f / cam->samples_per_pixel; // here  for now

	int32_t y = 0;
	while (y < cam->image_height)
	{
		int32_t x = 0;
		while (x < cam->image_width)
		{
			t_v3 color = {};
			int32_t sample;
			sample = 0;
			t_v3 incoming_light = {};
			uint32_t seed = (y * cam->image_width + x) + g_frame_num * 719393;
			while (sample < cam->samples_per_pixel)
			{
				ray.direction = get_ray_direction(cam, x, y, &seed);
				t_v3 hit_color = trace(ray, &spheres);
				// hit_color = V3_ADD(hit_color, light_intercetion())
				incoming_light = V3_ADD(incoming_light, hit_color);
				++sample;
			}
			color = f32_mul_v3(pixel_sample_scale, incoming_light);
			// color = get_ray_direction(&cam, x, y, x + y * cam.image_height);
			color = accumulate(rgb_u32_to_float(*out), color);
			*out++ = rgb_pack4x8(v3_clamp(color)); // maybe dont need clamp or do it in color correction
			++x;
		}
		++y;
	}
	++g_frame_num;
	// return (0);
}
bool init_camera_for_frame(t_minirt *minirt, t_camera *cam);

#include <stdio.h>

// static int32_t new_width = WINDOW_WIDTH;
// static int32_t new_height = WINDOW_HEIGHT;

#define ACCUMULATE_MAX 1024

void per_frame(void * param)
{
	static uint32_t accumulate_count = 0;
	static t_camera frame_cam = {};
	t_minirt *minirt;
	mlx_t *mlx;

	minirt = (t_minirt *)param;
	mlx = minirt->mlx;
	if (minirt->image->width != minirt->mlx->width || minirt->image->height != minirt->mlx->height)
	{
		mlx_resize_image(minirt->image, minirt->mlx->width, minirt->mlx->height);
		recalculate_cam = true;
	}
	if (recalculate_cam == true)
	{
		recalculate_cam = init_camera_for_frame(minirt, &minirt->base_cam);
		accumulate_count = 0;
		frame_cam = minirt->base_cam;
	}


	if (accumulate_count < ACCUMULATE_MAX)
	{
		render(&minirt->scene, &frame_cam, (uint32_t *)minirt->image->pixels);
		accumulate_count += frame_cam.samples_per_pixel;
	}
	// printf("hit: %i\tmiss: %i\n", hit, miss);

	printf("delta: %f\n", minirt->mlx->delta_time);
}

// static
// void on_resize(int32_t width, int32_t height, void *param)
// {
// 	t_minirt *minirt;

// 	minirt = (t_minirt *)param;
// 	new_width = width;
// 	new_height = height;
// 	// mlx_resize_image(minirt->image, width, height);

// }

static int	run_minirt(t_minirt *minirt, char **argv)
{
	if (init_minirt(minirt, argv) == FAIL)
		return (FAIL);
	mlx_set_window_size(minirt->mlx, WINDOW_WIDTH, WINDOW_HEIGHT);
	mlx_set_setting(MLX_STRETCH_IMAGE, false);

	// mlx_resize_hook(minirt->mlx, &on_resize, minirt);
	mlx_key_hook(minirt->mlx, &key_hook, minirt);
	mlx_loop_hook(minirt->mlx, &per_frame, minirt);

	ft_memset(minirt->image->pixels, 0, minirt->image->height * minirt->image->width * sizeof(uint32_t));
	base_init_cam(minirt, &minirt->base_cam);

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
// static
// void on_resize(int32_t width, int32_t height, void *param)
// {
// 	t_minirt *minirt;

// 	minirt = (t_minirt *)param;
// 	new_width = width;
// 	new_height = height;
// 	// mlx_resize_image(minirt->image, width, height);

// }
