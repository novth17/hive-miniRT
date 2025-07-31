#include "../inc/mini_rt.h"
#include "../inc/rt_math.h"
#include "../test_stuff/MLX42.h"
#include "../inc/object.h"

bool g_recalculate_cam = true;
uint32_t g_accummulated_frames = 0;


#include <float.h>
#include <stdint.h>
#include <math.h>

// static
// float ExactLinearTosRGB(float L)
// {
// 	float S;

// 	if (L < 0.0f)
// 	{
// 		L = 0.0f;
// 	}

// 	if (L > 1.0f)
// 	{
// 		L = 1.0f;
// 	}

// 	S = L * 12.92;
// 	if (L > 0.0031308)
// 	{
// 		S = 1.055F*pow(L, 1.0f/2.4f) - 0.055f;
// 	}
// 	return (S);
// }



static int	run_minirt(t_minirt *minirt, char **argv)
{
	if (init_minirt(minirt, argv) == FAIL)
		return (FAIL);


	base_init_cam(&minirt->scene.camera);
	minirt->scene.light_strength_mult = 1;
	minirt->scene.use_point_light = false;

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
