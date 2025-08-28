/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 19:13:28 by ltaalas           #+#    #+#             */
/*   Updated: 2025/08/28 19:13:28 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

#ifdef MINIRT_BONUS

static
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
	resize_linear_color_buf(minirt);
	create_task_queue(&minirt->queue, minirt, frame_cam);
	if (create_worker_threads(minirt))
		minirt->core_count = 1;
}

#else

static
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
	resize_linear_color_buf(minirt);
	create_task_queue(&minirt->queue, minirt, frame_cam);
}

#endif

int	resize_linear_color_buf(t_minirt *minirt)
{
	t_v4	*buf;
	size_t	total_size;

	if (minirt->linear_color_buf != NULL)
	{
		free(minirt->linear_color_buf);
		minirt->linear_color_buf = NULL;
	}
	total_size = (minirt->image->width * minirt->image->height) * sizeof(*buf);
	buf = malloc(total_size);
	if (buf == NULL)
	{
		ft_putstr_fd("miniRT: Failed to resize linear color buf\n", 2);
		return (-1);
	}
	ft_memset(buf, 0, total_size);
	minirt->linear_color_buf = buf;
	return (0);
}

static
int	run_minirt(t_minirt *minirt, char **argv)
{
	if (init_minirt(minirt, argv) == FAIL)
		return (FAIL);
	base_init_cam(&minirt->scene.camera);
	init_camera_for_frame(minirt, &minirt->scene.camera);
	draw_background(minirt);
	minirt->stop_threads = false;
	minirt->render = false;
	set_defaults(minirt, argv[1]);
	minirt->accum_start_time = mlx_get_time();
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
	delete_minirt(&minirt);
	ft_dprintf(1, "Program ran successfully!\n");
	return (SUCCESS);
}
