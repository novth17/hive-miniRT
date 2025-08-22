#include "../inc/mini_rt.h"

static
void set_default_task(t_task_queue *queue, t_minirt *minirt, t_camera *cam)
{
	static t_task g_default_task = {};
	g_default_task.out = (uint32_t *)minirt->image->pixels;
	g_default_task.scene = &minirt->scene;
	g_default_task.cam = cam;
	g_default_task.x_start = 0;
	g_default_task.y_start = 0;
	g_default_task.x_end_plus_one = cam->image_width;
	g_default_task.y_end_plus_one = cam->image_height;
	queue[0].next_task_index = 0;
	queue[0].task_count = 1;
	queue[0].tasks = &g_default_task;
}

//
// #define MINIRT_BONUS // testing // define this at compile time

#ifdef MINIRT_BONUS

#define TILE_SIZE 6

static
void set_task(t_task *task, t_tile_info *info, t_minirt *minirt, t_camera *cam)
{
	task->cam = cam;
	task->scene = &minirt->scene;
	task->out = (uint32_t *)minirt->image->pixels;
	task->x_start = info->min_x;
	task->x_end_plus_one = info->one_past_max_x;
	task->y_start = info->min_y;
	task->y_end_plus_one = info->one_past_max_y;
}

static
void	set_tasks(t_task_queue *queue, t_tile_info *info, t_minirt *minirt, t_camera *cam)
{
	uint32_t	tile_y;
	uint32_t	tile_x;
	t_task		*task;

	tile_y = 0;
	while (tile_y < info->tile_count_y)
	{
		info->min_y = tile_y * info->tile_height;
		info->one_past_max_y = info->min_y + info->tile_height;
		if (info->one_past_max_y > (uint32_t)cam->image_height)
			info->one_past_max_y = cam->image_height;
		tile_x = 0;
		while (tile_x < info->tile_count_x)
		{
			task = queue->tasks + queue->task_count++;
			info->min_x = tile_x * info->tile_width;
			info->one_past_max_x = info->min_x + info->tile_width;
			if (info->one_past_max_x > (uint32_t)cam->image_width)
				info->one_past_max_x = cam->image_width;
			set_task(task, info, minirt, cam);
			++tile_x;
		}
		++tile_y;
	}
}

bool create_task_queue(t_minirt *minirt, t_camera *cam)
{
	t_tile_info		info;
	t_task_queue	*queue;

	queue = &minirt->queue;
	info.tile_width = cam->image_width / TILE_SIZE;
	info.tile_height = info.tile_width;
	info.tile_count_x = (cam->image_width + info.tile_width - 1) / info.tile_width;
	info.tile_count_y = (cam->image_height + info.tile_height - 1) / info.tile_height;
	info.total_tile_count = info.tile_count_x * info.tile_count_y;
	ft_memset(queue, 0, sizeof(*queue));
	if (minirt->core_count > 1)
		queue->tasks = malloc(sizeof(t_task) * info.total_tile_count);
	if (queue->tasks == NULL)
	{
		set_default_task(queue, minirt, cam);
		return (-1);
	}
	set_tasks(queue, &info, minirt, cam);
	if (queue->task_count != info.total_tile_count)
	{
		ft_putstr_fd("tile creation failed\n", 2);
		free(queue->tasks);
		queue->tasks = NULL;
		set_default_task(queue, minirt, cam);
	}
	return (0);
}



#else

bool create_task_queue(t_minirt *minirt, t_camera *cam)
{
	set_default_task(&minirt->queue, minirt, cam);
	return (0);
}

#endif
