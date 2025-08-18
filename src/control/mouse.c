#include "../../inc/mini_rt.h"
#include <algorithm>


/**
 * Callback function used to handle raw mouse movement.
 *
 * @param[in] xpos The mouse x position.
 * @param[in] ypos The mouse y position.
 * @param[in] param Additional parameter to pass on to the function.
 */
static double prev_x;
static double prev_y;
static const float rotation_speed = 0.3f;
static const t_v3 up = {.x = 0.0f, .y = 1.0f, .z = 0.0f};
static bool mouse_was_down = false;


void rotate(t_camera *cam, t_v2 delta)
{
	const t_v3 right_direction = cross(cam->right, up);
	const float pitch_delta = delta.y * rotation_speed;
	const float yaw_delta = delta.x * rotation_speed;

	float yaw_angle = tan(yaw_delta);
	float pitch_angle = tan(pitch_delta);
}

void mouse_hook(double xpos, double ypos, void* param)
{
	t_v2 delta;
	t_minirt *minirt;
	t_camera *cam;

	minirt = (t_minirt *)param;
	if (mlx_is_mouse_down(minirt->mlx, MLX_MOUSE_BUTTON_RIGHT))
	{
		if (mouse_was_down == false)
		{
			mouse_was_down = true;
			prev_x = xpos;
			prev_y = ypos;
			return ;
		}
		mlx_set_cursor_mode(minirt->mlx, MLX_MOUSE_HIDDEN);
		delta.x = (xpos - prev_x) * 0.002f;
		delta.y = (ypos - prev_y) * 0.002f;
		if (delta.x != 0.f || delta.y != 0.f)
		{
			rotate(&minirt->scene.camera, delta);
		}
	}
	mlx_set_cursor_mode(minirt->mlx, MLX_MOUSE_NORMAL);
}
