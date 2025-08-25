#include "rt_math.h"

/// @brief gives a random float between 0.0f and 1.0f
/// @param seed seed for the generation
/// @return random number
inline
float	random_float(uint32_t *seed)
{
	uint32_t	state;
	uint32_t	result;

	state = *seed * 747796405u + 2891336453u;
	result = ((state >> ((state >> 28u) + 4u)) ^ state) * 277803737u;
	*seed = state;
	return ((float)result / (float)UINT32_MAX);
}

t_v3	in_unit_sphere(uint32_t *seed)
{
	t_v3	result;

	result.x = random_float(seed) * 2.0f - 1.0f;
	result.y = random_float(seed) * 2.0f - 1.0f;
	result.z = random_float(seed) * 2.0f - 1.0f;
	result = unit_vector(result);
	return (result);
}

inline
t_v3	random_in_unit_disk(uint32_t *seed)
{
	t_v3	p;
	float	x;
	float	y;
	int		i;

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
	return (v3(0, 0, 0));
}

inline
t_v3	random_direction(uint32_t *seed)
{
	const float	x = random_float(seed);
	const float	y = random_float(seed);
	const float	z = random_float(seed);

	return (normalize(v3(x, y, z)));
}

inline
t_v2	sample_square_stratified(
	int i_s, int j_s,
	float recip_sqrt_spp, uint32_t *seed)
{
	t_v2	result;

	result.x = ((i_s + random_float(seed)) * recip_sqrt_spp) - 0.5f;
	result.y = ((j_s + random_float(seed)) * recip_sqrt_spp) - 0.5f;
	return (result);
}
