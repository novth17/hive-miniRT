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

	// light strength multiplier to affect distance not color
	base_init_cam(&minirt->scene.camera);
	init_camera_for_frame(minirt, &minirt->scene.camera);
	draw_background(minirt);
	minirt->scene.light_dist_mult = 1.0f;
	minirt->scene.use_point_light = true;
	minirt->recalculate_cam = 1;

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
