#include "mini_rt.h"
#include "thread.h"

#ifdef MINIRT_BONUS
# include <pthread.h>

void	set_defaults(t_minirt *minirt, char *scene_file_name)
{
	t_camera	*frame_cam;

	frame_cam = get_frame_cam();
	*frame_cam = minirt->scene.camera;
	minirt->scene_file_name = scene_file_name;
	minirt->core_count = get_core_count();
	minirt->scene.light_dist_mult = 1.0f;
	minirt->scene.use_point_light = true;
	minirt->recalculate_cam = 1;
	printf("core_count %i\n", minirt->core_count);
	create_task_queue(&minirt->queue, minirt, frame_cam);
	if (create_worker_threads(minirt))
		minirt->core_count = 1;
}

#else

void	set_defaults(t_minirt *minirt, char *scene_file_name)
{
	t_camera	*frame_cam;

	frame_cam = get_frame_cam();
	*frame_cam = minirt->scene.camera;
	minirt->scene_file_name = scene_file_name;
	minirt->core_count = get_core_count();
	minirt->scene.light_dist_mult = 1.0f;
	minirt->scene.use_point_light = true;
	minirt->recalculate_cam = 1;
	printf("core_count %i\n", minirt->core_count);
	create_task_queue(&minirt->queue, minirt, frame_cam);
}

#endif

static
int	run_minirt(t_minirt *minirt, char **argv)
{
	if (init_minirt(minirt, argv) == FAIL)
		return (FAIL);
	base_init_cam(&minirt->scene.camera);
	init_camera_for_frame(minirt, &minirt->scene.camera);
	draw_background(minirt);
	set_defaults(minirt, argv[1]);
	mlx_loop(minirt->mlx);
	minirt->stop_threads = true;
	minirt->render = false;
	mlx_terminate(minirt->mlx);
	return (SUCCESS);
}

int	main(int argc, char **argv)
{
	t_minirt	minirt;

	if (validate_input(argc, argv[1]) == FAIL)
		return (FAIL);
	if (run_minirt(&minirt, argv) == FAIL)
		return (FAIL);
	ft_dprintf(1, "success hihihaha congrats\n");
	delete_minirt(&minirt);
	return (SUCCESS);
}
