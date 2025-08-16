#include "../inc/mini_rt.h"
#include "../inc/rt_math.h"
#include "../test_stuff/MLX42.h"
#include "../inc/object.h"


#include <float.h>
#include <stdint.h>
#include <math.h>


static
int	run_minirt(t_minirt *minirt, char **argv)
{
	if (init_minirt(minirt, argv) == FAIL)
		return (FAIL);


	base_init_cam(&minirt->scene.camera);
	minirt->scene.light_strength_mult = 10;
	minirt->scene.use_point_light = true;

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
