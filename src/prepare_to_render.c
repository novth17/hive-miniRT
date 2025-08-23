#include "mini_rt.h"
#include "image_to_file.h"

static
void	resize_images(t_minirt *minirt, mlx_t *mlx, t_camera *frame_cam)
{
	if (mlx_resize_image(minirt->image, mlx->width, mlx->height) == false)
		ft_putstr_fd("miniRT: Failed to resize image\n", 2);
	if (mlx_resize_image(minirt->background, mlx->width, mlx->height) == false)
		ft_putstr_fd("miniRT: Failed to resize background\n", 2);
	recalculate_camera(minirt, frame_cam);
	draw_background(minirt);
	if (minirt->queue.tasks && minirt->queue.tasks != get_default_task())
		free(minirt->queue.tasks);
	resize_linear_color_buf(minirt);
	create_task_queue(&minirt->queue, minirt, frame_cam);

}

void	prepare_to_render(t_minirt *minirt, mlx_t *mlx,
		mlx_image_t *img, t_camera *frame_cam)
{
	set_title(minirt);
	if (minirt->write_image_to_file == true)
	{
		pixels_to_image_file(minirt);
		minirt->write_image_to_file = false;
	}
	if (check_movement_keys(&minirt->scene.camera, mlx, mlx->delta_time))
		minirt->recalculate_cam = true;
	mouse_control(minirt);
	if (img->width != (uint32_t)mlx->width
		|| img->height != (uint32_t)mlx->height)
	{
		resize_images(minirt, mlx, frame_cam);
	}
	if (minirt->recalculate_cam == true)
	{
		recalculate_camera(minirt, frame_cam);
	}
}
