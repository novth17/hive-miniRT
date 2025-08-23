#include "rt_math.h"

// @deprecated
inline
t_v2	sample_square(uint32_t *seed)
{
	t_v2	result;

	result.x = random_float(seed) - 0.5f;
	result.y = random_float(seed) - 0.5f;
	return (result);
}

// @deprecated
t_v2	random_point_in_circle(uint32_t *state)
{
	const float	angle = random_float(state) * M_PI * 2;
	const float	sqr_ran = square_root(random_float(state));
	t_v2		result;

	result = v2(cos(angle) * sqr_ran, sin(angle) * sqr_ran);
	return (result);
}

// @deprecated
inline
float	random_float_normal_dist(uint32_t *seed)
{
	const float	theta = 2 * 3.1415926 * random_float(seed);
	const float	rho = square_root(-2 * log(random_float(seed)));

	return (rho * cosf(theta));
}

// @deprecated
inline
t_v3	random_direction_normal_dist(uint32_t *seed)
{
	const float	x = random_float_normal_dist(seed);
	const float	y = random_float_normal_dist(seed);
	const float	z = random_float_normal_dist(seed);

	return (normalize(v3(x, y, z)));
}

// @deprecated
inline
t_v3	random_direction_in_hemisphere(const t_v3 normal, uint32_t *rng_seed)
{
	const t_v3	dir = random_direction_normal_dist(rng_seed);

	return (v3_mul_f32(dir, sign(dot(normal, dir))));
}
