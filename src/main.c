#include "../inc/mini_rt.h"
#include "../inc/rt_math.h"
#include "../test_stuff/MLX42.h"


#include <float.h>
#include <stdint.h>
#include <math.h>
#include <sched.h>
#include <unistd.h>

# ifdef MINIRT_BONUS
# include <pthread.h>

static
int	get_core_count(void)
{
	cpu_set_t	cpuset;
	pid_t		pid;

	pid = getpid();
	sched_getaffinity(pid, sizeof(cpuset), &cpuset);
	return (CPU_COUNT(&cpuset));
}

static
int		run_minirt(t_minirt *minirt, char **argv)
{
	t_camera *frame_cam;
	if (init_minirt(minirt, argv) == FAIL)
		return (FAIL);
	base_init_cam(&minirt->scene.camera);
	init_camera_for_frame(minirt, &minirt->scene.camera);
	draw_background(minirt);

	frame_cam = get_frame_cam();
	*frame_cam = minirt->scene.camera;
	minirt->core_count = get_core_count();
	printf("core_count %i\n", minirt->core_count);
	minirt->scene.light_dist_mult = 1.0f;
	minirt->scene.use_point_light = true;
	minirt->recalculate_cam = 1;
	create_task_queue(minirt, get_frame_cam());
	if (create_worker_threads(minirt))
		minirt->core_count = 1;
	mlx_loop(minirt->mlx);
	minirt->stop_threads = true;
	minirt->render = false;
	mlx_terminate(minirt->mlx);
	return (SUCCESS);
}

#else
int get_core_count(void)
{
	return (1);
}


static
int		run_minirt(t_minirt *minirt, char **argv)
{
	if (init_minirt(minirt, argv) == FAIL)
		return (FAIL);
	base_init_cam(&minirt->scene.camera);
	init_camera_for_frame(minirt, &minirt->scene.camera);
	draw_background(minirt);

	minirt->core_count = get_core_count();
	printf("core_count %i\n", minirt->core_count);
	minirt->scene.light_dist_mult = 1.0f;
	minirt->scene.use_point_light = true;
	minirt->recalculate_cam = 1;
	mlx_loop(minirt->mlx);
	mlx_terminate(minirt->mlx);
	return (SUCCESS);
}
#endif

int	main(int argc, char **argv)
{
	t_minirt	minirt;

	if (validate_input(argc, argv[1]) == FAIL)
		return (FAIL);
	run_minirt(&minirt, argv);
	ft_dprintf(1, "success hihihaha congrats\n");
	delete_minirt(&minirt);
	return (SUCCESS);
}
