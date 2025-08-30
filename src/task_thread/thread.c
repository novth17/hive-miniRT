/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 19:13:47 by ltaalas           #+#    #+#             */
/*   Updated: 2025/08/29 17:06:33 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "thread.h"

#ifdef MINIRT_BONUS

static
bool	get_and_render_tile(volatile t_task_queue *queue)
{
	t_task		task;
	uint32_t	work_order_index;

	work_order_index = atomic_fetch_add(&queue->next_task_index, 1);
	if (work_order_index >= queue->task_count)
	{
		return (false);
	}
	task = queue->tasks[work_order_index];
	render_tile(task);
	atomic_fetch_add(&queue->tiles_retired_count, 1);
	return (true);
}

static
void	*render_thread(void *param)
{
	t_minirt		*minirt;
	t_task_queue	*queue;

	minirt = (t_minirt *)param;
	queue = &minirt->queue;
	while (!minirt->stop_threads)
	{
		if (minirt->render == true)
		{
			while (get_and_render_tile(queue))
				;
		}
		usleep(100);
	}
	return (NULL);
}

bool	create_worker_threads(t_minirt *minirt)
{
	const uint32_t	thread_count = minirt->core_count;
	pthread_t		thread_handle;
	uint32_t		i;

	if (minirt->core_count > 1)
	{
		i = 0;
		while (i < thread_count)
		{
			if (pthread_create(&thread_handle, NULL, &render_thread, minirt))
			{
				minirt->stop_threads = true;
				ft_putstr_fd("miniRT: thread creation failed\n"
					"Using main thread for rendering\n", 2);
				return (-1);
			}
			pthread_detach(thread_handle);
			++i;
		}
	}
	return (0);
}

int	get_core_count(void)
{
	cpu_set_t	cpuset;
	pid_t		pid;

	pid = getpid();
	sched_getaffinity(pid, sizeof(cpuset), &cpuset);
	return (CPU_COUNT(&cpuset));
}
#else

int	get_core_count(void)
{
	return (1);
}

#endif
