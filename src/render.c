#include "mini_rt.h"

static uint32_t	g_accumulated_frames = 0;

// lerp between according to weight
static inline
t_v4	accumulate(const t_v4 old_color, const t_v4 new_color)
{
	const float	weight = 1.0 / (g_accumulated_frames + 1);
	t_v4		accumulated_average;

	accumulated_average.r = old_color.r * (1 - weight) + new_color.r * weight;
	accumulated_average.g = old_color.g * (1 - weight) + new_color.g * weight;
	accumulated_average.b = old_color.b * (1 - weight) + new_color.b * weight;
	accumulated_average.a = old_color.a * (1 - weight) + new_color.a * weight;
	return (accumulated_average);
}

void	render_tile(const t_task task)
{
	t_v4		color;
	uint32_t	pixel_index;
	uint32_t	seed;
	uint32_t	x;
	uint32_t	y;

	y = task.y_start;
	while (y < task.y_end_plus_one)
	{
		x = task.x_start;
		while (x < task.x_end_plus_one)
		{
			pixel_index = (y * task.cam->image_width + x);
			seed = pixel_index + g_accumulated_frames * 792858;
			color = sample_pixel(task.scene, task.cam, (t_cord){x, y}, seed);
			if (task.linear_buf != NULL)
			{
				color =	accumulate(task.linear_buf[pixel_index], color);
				task.linear_buf[pixel_index] = color;
			}
			else
				color = accumulate(exact_unpack(task.out[pixel_index]), color);
			task.out[pixel_index] = exact_pack(color);
			++x;
		}
		++y;
	}
}

#ifdef MINIRT_BONUS

void	per_frame(void *param)
{
	t_camera		*frame_cam;
	t_minirt		*minirt;
	mlx_t			*mlx;
	t_task_queue	*queue;

	minirt = (t_minirt *)param;
	mlx = minirt->mlx;
	frame_cam = get_frame_cam();
	prepare_to_render(minirt, mlx, minirt->image, frame_cam);
	queue = &minirt->queue;
	queue->tiles_retired_count = 0;
	queue->next_task_index = 0;
	atomic_exchange(&minirt->render, true);
	while (queue->tiles_retired_count < queue->task_count)
	{
		usleep(1000);
	}
	atomic_exchange(&minirt->render, false);
	++g_accumulated_frames;
}

#else

void	per_frame(void *param)
{
	t_camera		*frame_cam;
	t_minirt		*minirt;
	mlx_t			*mlx;
	t_task_queue	*queue;

	minirt = (t_minirt *)param;
	mlx = minirt->mlx;
	frame_cam = get_frame_cam();
	prepare_to_render(minirt, mlx, minirt->image, frame_cam);
	queue = &minirt->queue;
	queue->tiles_retired_count = 0;
	queue->next_task_index = 0;
	render_tile(queue->tasks[0]);
	++g_accumulated_frames;
}

#endif

void	recalculate_camera(t_minirt *minirt, t_camera *frame_cam)
{
	init_camera_for_frame(minirt, &minirt->scene.camera);
	*frame_cam = minirt->scene.camera;
	minirt->recalculate_cam = false;
	g_accumulated_frames = 0;
}
