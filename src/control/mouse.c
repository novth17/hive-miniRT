#include "../../inc/mini_rt.h"

static int32_t		g_prev_x;
static int32_t		g_prev_y;
static const float	g_rotation_speed = 0.002f;
static float		g_total_rotation_x = 0.f;
static float		g_total_rotation_y = 0.f;

void	rotate_view(t_camera *cam)
{
	static const t_v3	y_axis = {.x = 0.0f, .y = 1.0f, .z = 0.0f};
	t_v3				view;

	view = rotate(cam->base_direction, y_axis, g_total_rotation_x);
	view = normalize(view);
	cam->right = normalize(cross(y_axis, view));
	view = rotate(view, cam->right, g_total_rotation_y);
	cam->lookat = normalize(view);
	cam->vup = cross(cam->lookat, cam->right);
	cam->vup = normalize(cam->vup);
}

#define DEG_90 1.5707963268f

void	mouse_moved(t_minirt *minirt, t_v2 delta)
{
	g_total_rotation_x += delta.x * g_rotation_speed;
	g_total_rotation_y += delta.y * g_rotation_speed;
	if (g_total_rotation_y > DEG_90)
		g_total_rotation_y = DEG_90;
	if (g_total_rotation_y < -DEG_90)
		g_total_rotation_y = -DEG_90;
	rotate_view(&minirt->scene.camera);
	minirt->recalculate_cam = true;
}

void	mouse_control(t_minirt *minirt)
{
	t_v2	delta;
	int32_t	x;
	int32_t	y;

	mlx_get_mouse_pos(minirt->mlx, &x, &y);
	if (mlx_is_mouse_down(minirt->mlx, MLX_MOUSE_BUTTON_RIGHT))
	{
		mlx_set_cursor_mode(minirt->mlx, MLX_MOUSE_DISABLED);
		delta.x = (float)(g_prev_x - x);
		delta.y = (float)(g_prev_y - y);
		if (delta.x != 0.f || delta.y != 0.f)
			mouse_moved(minirt, delta);
	}
	else
	{
		mlx_set_cursor_mode(minirt->mlx, MLX_MOUSE_NORMAL);
	}
	g_prev_x = x;
	g_prev_y = y;
}

void	reset_rotation_angles(void)
{
	g_total_rotation_x = 0.0f;
	g_total_rotation_y = 0.0f;
}
