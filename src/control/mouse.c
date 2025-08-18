#include "../../inc/mini_rt.h"



static float prev_x;
static float prev_y;
static const float rotation_speed = 0.002f;
static const t_v3 up = {.x = 0.0f, .y = 1.0f, .z = 0.0f};
static bool mouse_was_down = false;

t_v4 quaternion_from_axis_angle(t_v3 axis, float angle)
{
    const float c = sin(angle / 2.0f);
	t_v4 result;
    // Formula from http://www.euclideanspace.com/maths/geometry/rotations/conversions/angleToQuaternion/
    result.w = cos(angle / 2.0f);
    result.x = c * axis.x;
    result.y = c * axis.y;
    result.z = c * axis.z;
	return (result);
}

// t_v4 quaternion_from_x(float angle)
// {
//     return (quaternion_from_axis_angle(v3(1.0f, 0, 0), angle));
// }
// t_v4 quaternion_from_y(float angle)
// {
//     return (quaternion_from_axis_angle(v3(0, 1.0f, 0), angle));
// }

// rotete direction around axis by angle
t_v3 rotate(t_v3 direction, t_v3 axis, float angle)
{
	const t_v4 rotation_q = quaternion_from_axis_angle(axis, angle);
	const t_v4 conjugate_q = quaternion_conjugate(rotation_q);
	t_v4 w;
	t_v3 result;

	w = q_mul_v3(rotation_q, direction);
	w = q_mul_q(w, conjugate_q);
	result.x = w.x;
	result.y = w.y;
	result.z = w.z;
	return (result);
}

void rotate_view(t_camera *cam, t_v2 delta)
{
	const t_v4 yaw = quaternion_from_axis_angle(up, delta.x * rotation_speed);
	const t_v4 pitch = quaternion_from_axis_angle(cam->right, delta.y * rotation_speed);
	const t_v4 rotation_q = q_mul_q(yaw, pitch);
	const t_v4 conjugate_q = quaternion_conjugate(rotation_q);
	t_v4 w;

	w = q_mul_v3(rotation_q, cam->lookat);
	w = q_mul_q(w, conjugate_q);
	cam->lookat.x = w.x;
	cam->lookat.y = w.y;
	cam->lookat.z = w.z;
}

// void mouse_control(t_minirt *minirt)
// {
// 	int32_t x;
// 	int32_t y;
// 	t_v2 screen_middle;
// 	t_v2 delta;

// 	if (mlx_is_mouse_down(minirt->mlx, MLX_MOUSE_BUTTON_RIGHT))
// 	{
// 		mlx_set_cursor_mode(minirt->mlx, MLX_MOUSE_DISABLED);
// 		if (mouse_was_down == true)
// 		{
// 			mlx_get_mouse_pos(minirt->mlx, &x, &y);
// 			delta.x = (float)x - prev_x;
// 			delta.y = (float)y - prev_y;
// 			printf("prev_x %f, prev_y %f, x %i, y %i\ndelta_x %f, delta_y %f\n", prev_x, prev_y, x, y, delta.x, delta.y);
// 			if (delta.x != 0.f || delta.y != 0.f)
// 			{
// 				rotate_view(&minirt->scene.camera, delta);
// 				minirt->recalculate_cam = true;
// 			}
// 		}
// 		mouse_was_down = true;
// 		// mlx_set_mouse_pos(minirt->mlx, screen_middle.x, screen_middle.y);
// 	}
// 	else
// 	{
// 		mouse_was_down = false;
// 		mlx_set_cursor_mode(minirt->mlx, MLX_MOUSE_NORMAL);
// 	}
// 	prev_x = x;
// 	prev_y = y;
// }

void mouse_hook(double xpos, double ypos, void* param)
{
	t_v2 screen_middle;
	t_v2 delta;
	t_minirt *minirt;

	minirt = (t_minirt *)param;
	screen_middle.x = (float)minirt->mlx->width / 2.0f;
	screen_middle.y = (float)minirt->mlx->height / 2.0f;
	if (mlx_is_mouse_down(minirt->mlx, MLX_MOUSE_BUTTON_RIGHT))
	{
		mlx_set_cursor_mode(minirt->mlx, MLX_MOUSE_DISABLED);
		if (mouse_was_down == true)
		{
			delta.x = (xpos - prev_x);
			delta.y = (ypos - prev_y);
			printf("prev_x %f, prev_y %f, x %f, y %f\ndelta_x %f, delta_y %f\n", prev_x, prev_y, xpos, xpos, delta.x, delta.y);
			if (delta.x != 0.f || delta.y != 0.f)
			{
				rotate_view(&minirt->scene.camera, delta);
				minirt->recalculate_cam = true;
				init_camera_for_frame(minirt, &minirt->scene.camera);
			}
		}
		mouse_was_down = true;
		// mlx_set_mouse_pos(minirt->mlx, screen_middle.x, screen_middle.y);
	}
	else
	{
		mouse_was_down = false;
		mlx_set_cursor_mode(minirt->mlx, MLX_MOUSE_NORMAL);
	}
	prev_x = xpos;
	prev_y = ypos;
}
