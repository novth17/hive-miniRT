#include "task.h"
#include "camera.h"

t_task	*get_default_task(void)
{
	static t_task	default_task = {};

	return (&default_task);
}

t_camera	*get_frame_cam(void)
{
	static t_camera	frame_cam = {};

	return (&frame_cam);
}
