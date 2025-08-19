#include "../../inc/mini_rt.h"



static int32_t		prev_x;
static int32_t		prev_y;
static const float	rotation_speed = 0.002f;
static float		total_rotation_x = 0.f;
static float		total_rotation_y = 0.f;
static bool			mouse_was_down = false;

void rotate_view(t_camera *cam)
{
	static const t_v3	y_axis = {.x = 0.0f, .y = 1.0f, .z = 0.0f};
	t_v3				view;

	view = rotate(cam->base_direction, y_axis, total_rotation_x);
	view = normalize(view);
	cam->right = normalize(cross(y_axis, view));
	view = rotate(view, cam->right, total_rotation_y);
	cam->lookat = normalize(view);
	cam->vup = cross(cam->lookat, cam->right);
	cam->vup = normalize(cam->vup);
}

#define DEG_90 1.5707963268f

void	mouse_moved(t_minirt *minirt, t_v2 delta)
{
	total_rotation_x += delta.x * rotation_speed;
	total_rotation_y += delta.y * rotation_speed;
	if (total_rotation_y > DEG_90)
		total_rotation_y = DEG_90;
	if (total_rotation_y < -DEG_90)
		total_rotation_y = -DEG_90;
	rotate_view(&minirt->scene.camera);
	minirt->recalculate_cam = true;
}

void mouse_control(t_minirt *minirt)
{
	t_v2	delta;
	int32_t	x;
	int32_t	y;

	mlx_get_mouse_pos(minirt->mlx, &x, &y);
	if (mlx_is_mouse_down(minirt->mlx, MLX_MOUSE_BUTTON_RIGHT))
	{
		mlx_set_cursor_mode(minirt->mlx, MLX_MOUSE_DISABLED);
		if (mouse_was_down == true)
		{
			delta.x = (float)(prev_x - x);
			delta.y = (float)(prev_y - y);
			if (delta.x != 0.f || delta.y != 0.f)
				mouse_moved(minirt, delta);
		}
		mouse_was_down = true;
	}
	else
	{
		mouse_was_down = false;
		mlx_set_cursor_mode(minirt->mlx, MLX_MOUSE_NORMAL);
	}
	prev_x = x;
	prev_y = y;
}
