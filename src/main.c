#include "../inc/mini_rt.h"
#include "../inc/rt_math.h"
#include "../test_stuff/MLX42.h"
#include "../inc/object.h"

bool g_recalculate_cam = true;
uint32_t g_accummulated_frames = 0;


#include <float.h>
#include <stdint.h>
#include <math.h>

void base_init_cam(t_minirt *minirt, t_camera *cam);

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

// not used
static inline
t_v3 get_offset(uint32_t *seed)
{
	t_v3 result = {};
	result = in_unit_sphere(seed);
	return (result);
}

static int	run_minirt(t_minirt *minirt, char **argv)
{
	if (init_minirt(minirt, argv) == FAIL)
		return (FAIL);


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

