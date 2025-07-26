#include "../inc/mini_rt.h"
#include "../inc/rt_math.h"
#include "../test_stuff/MLX42.h"
#include "../inc/object.h"

bool g_recalculate_cam = true;
uint32_t g_accummulated_frames = 0;


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





// static inline
// t_ray init_ray(t_camera *cam, int32_t x, int32_t y);

// typedef struct
// {
// 	t_v3	position;
// 	float	radius;
// 	t_v3 	color;
// } t_sphere;

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





// static uint32_t rng_state = 25021129;

// inline
// uint32_t pcg_hash(uint32_t seed)
// {
//     const uint32_t state = seed * 747796405u + 2891336453u;
//     const uint32_t word = ((state >> ((state >> 28u) + 4u)) ^ state) * 277803737u;

//     return ((word >> 22u) ^ word);
// }







// static uint32_t seed = 124124;




// not used currently might never be wooooo



inline
float random_float_normal_dist(uint32_t *seed)
{
	const float theta = 2 * M_PI + random_float(seed);
	const float rho = square_root(-2 * log(random_float(seed)));
	return (rho * cos(theta));
}

inline
t_v3 random_direction_normal_dist(uint32_t *seed)
{
	const float x = random_float_normal_dist(seed);
	const float y = random_float_normal_dist(seed);
	const float z = random_float_normal_dist(seed);

	return (normalize((t_v3){x, y, z}));
}

inline
t_v3 random_direction(uint32_t *seed)
{
	const float x = random_float(seed);
	const float y = random_float(seed);
	const float z = random_float(seed);

	return (normalize((t_v3){x, y, z}));
}

t_v3 random_direction_in_hemisphere(const t_v3 normal, uint32_t *rng_seed)
{
	const t_v3 dir = random_direction_normal_dist(rng_seed);
	return (v3_mul_f32(dir, sign(dot(normal, dir))));
}

// typedef struct
// {
// 	t_v3 location;
// 	t_v3 color;
// 	/* data */
// } t_new_light;







void base_init_cam(t_minirt *minirt, t_camera *cam);

// uint32_t sample_pixel()
// {

// }










// t_v3 sample_pixel(const t_scene *scene, const t_camera *restrict cam, const uint32_t x, const uint32_t y)
// {
// 	// for testing without parser
// ///////////////////////////////////////////
// 	static const int count = 3;
// 	t_spheres spheres;
// 	t_sphere arr[count];
// 	spheres.count = count;
// 	spheres.arr = arr;
// 	arr[0].center = v3(0, 0, -1);
// 	arr[0].radius = 0.5f;
// 	arr[0].color = v3(1, 0, 0);
// 	arr[1].center = v3(0, -55, 0);
// 	arr[1].radius = 54.0f;
// 	arr[1].color = v3(1, 1, 1);

// 	arr[2].center = v3(1, 1, 0);
// 	arr[2].radius = 0.3f;
// 	arr[2].color = v3(0, 1, 1);
// /////////////////////////////////////////
// 	t_ray ray;
// 	t_v3 color;
// 	int32_t sample;
// 	t_v3 incoming_light;
// 	uint32_t seed;

// 	seed = (y * cam->image_width + x) + g_accummulated_frames * 792858;
// 	incoming_light = v3(0, 0, 0);
// 	sample = 0;
// 	// const float strength = 2.0f;
// 	// 	t_v2 jitter = random_point_in_circle(&seed);
// 	// 	jitter.x =  jitter.x * strength / x;
// 	// 	jitter.y = jitter.y * strength / x;
// 	// t_v3 jittered_view = cam->camera_center;
// 	ray.origin = cam->camera_center;
// 	while (sample < cam->samples_per_pixel)
// 	{
// 		// ray.origin.x = cam->camera_center.x * test.x;
// 		// ray.origin.y = cam->camera_center.y * test.y;
// 		// ray.origin.z = cam->camera_center.z;
// 		if (cam->defocus_angle > 0)
// 			ray.origin = defocus_disk_sample(cam, &seed);
// 		ray.direction = get_ray_direction(cam, ray, (t_cord){x, y}, &seed);
// 		color = trace(ray, scene, &seed);
// 		incoming_light = V3_ADD(incoming_light, color);
// 		++sample;
// 	}
// 	color = f32_mul_v3(cam->pixel_sample_scale, incoming_light);
// 	return (color);
// }

// not used
static inline
t_v3 get_offset(uint32_t *seed)
{
	t_v3 result = {};
	result = in_unit_sphere(seed);
	return (result);
}


bool init_camera_for_frame(t_minirt *minirt, t_camera *cam);


static int	run_minirt(t_minirt *minirt, char **argv)
{
	if (init_minirt(minirt, argv) == FAIL)
		return (FAIL);

	// mlx_resize_hook(minirt->mlx, &on_resize, minirt);
	// mlx_key_hook(minirt->mlx, &key_hook, minirt);
	// mlx_scroll_hook(minirt->mlx, &scroll_hook, minirt);
	// mlx_loop_hook(minirt->mlx, &per_frame, minirt);
	base_init_cam(minirt, &minirt->scene.camera);

	mlx_loop(minirt->mlx);
	mlx_terminate(minirt->mlx);
	return (SUCCESS);
}

int	main(int argc, char **argv)
{
	t_minirt minirt;

	if (validate_input(argc, argv[1]) == FAIL)
		return (FAIL);
	run_minirt(&minirt, argv);
	ft_dprintf(1, "success hihihaha congrats\n");
	delete_minirt(&minirt);
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
