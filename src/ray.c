#include "mini_rt.h"
#include "types.h"
#include "rt_math.h"
#include "camera.h"

// for calculating the defocus blur effect
inline
t_v3	defocus_disk_sample(const t_camera *restrict cam, uint32_t *rng_state)
{
	const t_point3	point = random_in_unit_disk(rng_state);
	const t_v3		x_defocus = f32_mul_v3(point.x, cam->defocus_disk_u);
	const t_v3		y_defocus = f32_mul_v3(point.y, cam->defocus_disk_v);
	t_v3			result;

	result = V3_ADD(cam->camera_center, x_defocus);
	result = V3_ADD(result, y_defocus);
	return (result);
}

inline
t_ray	get_ray(const t_camera *restrict cam,
	t_cord cord, t_cord strati, uint32_t *seed)
{
	const t_v2	offset = sample_square_stratified(
			strati.x,
			strati.y,
			cam->recip_sqrt_spp,
			seed);
	t_v3		x_delta;
	t_v3		y_delta;
	t_v3		pixel_sample;
	t_ray		ray;

	x_delta = f32_mul_v3(cord.x + offset.x, cam->pixel_delta_u);
	y_delta = f32_mul_v3(cord.y + offset.y, cam->pixel_delta_v);
	pixel_sample = V3_ADD(cam->pixel00_loc, V3_ADD(x_delta, y_delta));
	ray.origin = cam->camera_center;
	if (cam->defocus_angle > 0.0f)
		ray.origin = defocus_disk_sample(cam, seed);
	ray.direction = normalize(V3_SUB(pixel_sample, ray.origin));
	return (ray);
}

// Return true if the vector is close to zero in all dimensions.
static inline
bool	near_zero(const t_v3 a)
{
	const float	s = 1e-8f;

	return ((fabsf(a.x) < s) && (fabsf(a.y) < s) && (fabsf(a.z) < s));
}

// not normalizing the random bounce direction and the final ray direction
// causes the image to be noticably brighter
// it is also quite a bit faster to render
// the light seems to not spread properly

// inner product (or dot product) gives us the angle difference between
// the normal at the hit point and the direction of the ray
// that angle times 2 is the direction we want the mirror reflection to go
inline
t_ray	calculate_next_ray(const t_hit *restrict rec,
	t_ray ray, bool is_specular_bounce, uint32_t *seed)
{
	t_v3	scatter;
	t_v3	random_bounce;
	t_v3	pure_bounce;
	float	two_times_dot;
	float	real_diffuse;

	scatter = in_unit_sphere(seed);
	scatter = v3_mul_f32(scatter, !near_zero(scatter));
	random_bounce = normalize(V3_ADD(rec->normal, scatter));
	two_times_dot = 2.0f * dot(ray.direction, rec->normal);
	pure_bounce = f32_mul_v3(two_times_dot, rec->normal);
	pure_bounce = v3_sub_v3(ray.direction, pure_bounce);
	real_diffuse = rec->mat.diffuse * is_specular_bounce;
	ray.direction = v3_lerp(random_bounce, real_diffuse, pure_bounce);
	ray.origin = rec->position;
	return (ray);
}
