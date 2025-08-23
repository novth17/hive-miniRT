#ifndef TASK_H
# define TASK_H

# include <stdbool.h>
# include "camera.h"

# ifdef MINIRT_BONUS
#  include <stdatomic.h>
# endif

typedef struct s_mini_rt	t_minirt;
typedef struct s_scene		t_scene;

typedef struct s_task
{
	uint32_t	*out;
	t_scene		*scene;
	t_camera	*cam;
	uint32_t	x_start;
	uint32_t	x_end_plus_one;
	uint32_t	y_start;
	uint32_t	y_end_plus_one;
}	t_task;

# ifdef MINIRT_BONUS

typedef struct s_task_queue
{
	uint32_t						task_count;
	t_task							*tasks;
	volatile atomic_uint_fast32_t	next_task_index;
	volatile atomic_uint_fast32_t	tiles_retired_count;
}	t_task_queue;

# else

typedef struct s_task_queue
{
	uint32_t			task_count;
	t_task				*tasks;
	volatile uint32_t	next_task_index;
	volatile uint32_t	tiles_retired_count;
}	t_task_queue;

# endif

typedef struct s_tile_info
{
	uint32_t	tile_width;
	uint32_t	tile_height;
	uint32_t	tile_count_x;
	uint32_t	tile_count_y;
	uint32_t	total_tile_count;

	uint32_t	min_y;
	uint32_t	min_x;
	uint32_t	one_past_max_x;
	uint32_t	one_past_max_y;
}	t_tile_info;

t_task	*get_default_task(void);
bool	create_task_queue(t_task_queue *queue, t_minirt *minirt, t_camera *cam);
bool	get_and_render_tile(t_task_queue *queue);

#endif
