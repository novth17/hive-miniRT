#include "../inc/mini_rt.h"
#include "../inc/rt_math.h"
#include "../test_stuff/MLX42.h"
#include "../inc/object.h"

static bool g_recalculate_cam = true;
static uint32_t g_accummulated_frames = 0;


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

typedef struct s_hit_record
{
	t_v3 position; // p
	t_v3 normal;
	float distance; // t
	bool front_face; // maybe not needed;
	t_v3 color;
	bool did_hit; // can be removed later
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
t_hit create_sphere_hit_record(const t_ray ray, const t_sphere sp, const float root)
{
	t_hit rec;

	rec.did_hit = true;
	rec.distance = root;
	rec.position = at(ray, rec.distance);
	rec.normal = v3_div_f32(V3_SUB(rec.position, sp.center), sp.radius);
	rec.color = sp.color;
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

static inline
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
t_v3 random_in_unit_disk(uint32_t *seed)
{
	t_v3 p;
	float x;
	float y;
	int i;

	i = 0;
	while (i < 100)
	{
		x = random_float(seed) * 2.0f - 1.0f;
		y = random_float(seed) * 2.0f - 1.0f;
		p = v3(x, y, 0);
		if (length_sq(p) < 1)
			return (p);
		++i;
	}
	return (v3(-0.2, 0.3, 0));
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

typedef struct main
{
	t_v3 location;
	t_v3 color;
	/* data */
} t_new_light;



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


static inline
t_v3 trace(t_ray ray, const t_scene *scene, uint32_t *seed) // change to all objects or scene;
{

	t_v3 ambient = f32_mul_v3(scene->ambient.ratio, scene->ambient.color);
	// t_v3 point_ligth_loc = v3(2, 2, 2);
	t_v3 point_light_color = f32_mul_v3(scene->light.bright_ratio, scene->light.color);
	t_sphere point_light_sphere = {.color = v3(20, 20, 20), .center = scene->light.origin, .radius = 0.05f};
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
		// printf("nohit\n");
		return (v3_mul_v3(ambient, rec.color));
		// color = v3_mul_v3(f32_mul_v3(0.5, V3_ADD(rec.normal, v3(1, 1, 1))), color); // reflectivity of material * color calc
	}
		// return (v3(.1,.1,.1));

	// t_v3 hit_color = v3_mul_v3(ambient, ray_color(ray, &spheres));
	// hit_color = V3_ADD(hit_color, light_intercetion())



	// not hit = background color
	t_v3 unit_direction = unit_vector(ray.direction);
	float a = 0.5 * (unit_direction.y + 1.0f);

	t_v3 result;
	result = f32_mul_v3(1.0 - a, v3(1.0, 1.0, 1.0));
	result = v3_add_v3(result, f32_mul_v3(a, v3(0.5, 0.7, 1.0)));
	// incoming_ligth = result; // for now
	return (result);
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

t_v2 random_point_in_circle(uint32_t *state)
{
	const float angle = random_float(state) * M_PI * 2;
	const float sqr_ran = square_root(random_float(state));
	t_v2 result;

	result = v2(cos(angle) * sqr_ran, sin(angle) * sqr_ran);
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

t_v3 defocus_disk_sample(const t_camera *restrict cam, uint32_t *rng_state)
{
	const t_point3 point = random_in_unit_disk(rng_state);
	t_v3 result;

	result = V3_ADD(cam->camera_center, f32_mul_v3(point.x, cam->defocus_disk_u));
	result = V3_ADD(result, f32_mul_v3(point.y, cam->defocus_disk_v));
	return (result);
}


t_v3 sample_pixel(const t_scene *scene, const t_camera *restrict cam, const uint32_t x, const uint32_t y)
{
	// for testing without parser
///////////////////////////////////////////
	static const int count = 3;
	t_spheres spheres;
	t_sphere arr[count];
	spheres.count = count;
	spheres.arr = arr;
	arr[0].center = v3(0, 0, -1);
	arr[0].radius = 0.5f;
	arr[0].color = v3(1, 0, 0);
	arr[1].center = v3(0, -55, 0);
	arr[1].radius = 54.0f;
	arr[1].color = v3(1, 1, 1);

	arr[2].center = v3(1, 1, 0);
	arr[2].radius = 0.3f;
	arr[2].color = v3(0, 1, 1);
/////////////////////////////////////////
	t_ray ray;
	t_v3 color;
	int32_t sample;
	t_v3 incoming_light;
	uint32_t seed;

	seed = (y * cam->image_width + x) + g_accummulated_frames * 792858;
	incoming_light = v3(0, 0, 0);
	sample = 0;
	// const float strength = 2.0f;
	// 	t_v2 jitter = random_point_in_circle(&seed);
	// 	jitter.x =  jitter.x * strength / x;
	// 	jitter.y = jitter.y * strength / x;
	// t_v3 jittered_view = cam->camera_center;
	ray.origin = cam->camera_center;
	while (sample < cam->samples_per_pixel)
	{
		// ray.origin.x = cam->camera_center.x * test.x;
		// ray.origin.y = cam->camera_center.y * test.y;
		// ray.origin.z = cam->camera_center.z;
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

static
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
bool init_camera_for_frame(t_minirt *minirt, t_camera *cam);


// static int32_t new_width = WINDOW_WIDTH;
// static int32_t new_height = WINDOW_HEIGHT;

// #define ACCUMULATE_MAX (1024 * 8)

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
		g_recalculate_cam = init_camera_for_frame(minirt, &minirt->scene.camera);
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

// static
// void on_resize(int32_t width, int32_t height, void *param)
// {
// 	t_minirt *minirt;

// 	minirt = (t_minirt *)param;
// 	new_width = width;
// 	new_height = height;
// 	// mlx_resize_image(minirt->image, width, height);

// }

void scroll_hook(double delta_x, double delta_y, void *param)
{
	t_minirt *minirt;
	(void)delta_x;

	minirt = (t_minirt *)param;
	if (delta_y != 0.0)
		g_recalculate_cam = true;
	if (mlx_is_key_down(minirt->mlx, MLX_KEY_LEFT_SHIFT))
	{
		if (delta_y > 0)
			minirt->scene.camera.focus_dist += 0.2f;
		else if (delta_y < 0)
		{
			minirt->scene.camera.focus_dist -= 0.2f;
			if (minirt->scene.camera.focus_dist < 0.02f)
				minirt->scene.camera.focus_dist = 0.02f;
		}
	}
	else if (mlx_is_key_down(minirt->mlx, MLX_KEY_LEFT_CONTROL))
	{
		if (delta_y > 0)
			minirt->scene.camera.defocus_angle += 0.2f;
		else if (delta_y < 0)
		{
			minirt->scene.camera.defocus_angle -= 0.2f;
			if (minirt->scene.camera.defocus_angle < 0.02f)
			minirt->scene.camera.defocus_angle = 0.02f;
		}
	}
	else
	{
		if (delta_y > 0 && minirt->scene.camera.fov > 5.0f)
			minirt->scene.camera.fov -= 5.0f;
		else if (delta_y < 0 && minirt->scene.camera.fov < 175.0f)
		{
			minirt->scene.camera.fov += 5.0f;
		}
	}
}

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
		g_recalculate_cam = true;
		if (data.key == MLX_KEY_W)
		{
			direction = f32_mul_v3(minirt->mlx->delta_time * speed, minirt->scene.camera.lookat);
			minirt->scene.camera.lookfrom = V3_ADD(minirt->scene.camera.lookfrom, direction);
		}
		else if (data.key == MLX_KEY_S)
		{
			direction = f32_mul_v3(minirt->mlx->delta_time * speed, minirt->scene.camera.lookat);
			minirt->scene.camera.lookfrom = V3_SUB(minirt->scene.camera.lookfrom, direction);
		}
	}
	// }
}

static int	run_minirt(t_minirt *minirt, char **argv)
{
	if (init_minirt(minirt, argv) == FAIL)
		return (FAIL);
	mlx_set_window_size(minirt->mlx, WINDOW_WIDTH, WINDOW_HEIGHT); // these are here so i dont touch the init file (too much)
	mlx_resize_image(minirt->image, WINDOW_WIDTH, WINDOW_HEIGHT);
	mlx_set_setting(MLX_STRETCH_IMAGE, false);

	// mlx_resize_hook(minirt->mlx, &on_resize, minirt);
	mlx_key_hook(minirt->mlx, &key_hook, minirt);
	mlx_scroll_hook(minirt->mlx, &scroll_hook, minirt);
	mlx_loop_hook(minirt->mlx, &per_frame, minirt);
	ft_memset(minirt->image->pixels, 0xFFFFFFFF, minirt->image->height * minirt->image->width * sizeof(uint32_t));
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
