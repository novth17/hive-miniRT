#include "../inc/rt_math.h"
#include "../inc/mini_rt.h"


// typedef struct
// {
// 	float	aspect_ratio; //
// 	int32_t image_width;
// 	int32_t image_height;
// 	int32_t samples_per_pixel;
// 	int32_t max_bounce; // for mandatory 1 or 2 i guess;

// 	float		vfov;
// 	t_point3 origin;
// 	t_point3 direction;
// 	t_v3	 vup;

// 	float focal_length;
// 	float viewport_height;
// 	float viewport_width;
// 	t_v3 camera_center;

// 	t_v3 viewport_u;
// 	t_v3 viewport_v;

// 	t_v3 pixel_delta_u;
// 	t_v3 pixel_delta_v;

// 	t_v3 viewport_upper_left;
// 	t_v3 pixel00_loc;


// } t_camera;

static inline
t_v3	viewport_top_left(const t_v3 camera_center, const t_v3 viewport_u, const t_v3 viewport_v, const float focal_length)
{
	t_v3	result;

	result = V3_SUB(camera_center, v3(0, 0, focal_length));
	result = V3_SUB(result, v3_div_f32(viewport_u, 2.0f));
	result = V3_SUB(result, v3_div_f32(viewport_v, 2.0f));
	return (result);
}

static inline
t_v3	pixel00_location(const t_v3 viewport_upper_left, const t_v3 pixel_delta_u, const t_v3 pixel_delta_v)
{
	t_v3	result;

	result = v3_add_v3(pixel_delta_u, pixel_delta_v);
	result = f32_mul_v3(0.5, result);
	result = v3_add_v3(viewport_upper_left, result);
	return (result);
}

void base_init_cam(t_minirt *minirt, t_camera *cam)
{
		// cam->aspect_ratio = 16.0 / 9.0;
		// cam->image_width = 400;
		cam->aspect_ratio = (float)minirt->image->width / minirt->image->height;
		cam->image_width = minirt->image->width;
		cam->image_height = (int32_t)(cam->image_width / cam->aspect_ratio);
		if (cam->image_height < 1)
			cam->image_height = 1;

		cam->samples_per_pixel = 1;


		cam->focal_length = 1.0f;
		cam->viewport_height = 2.0f;
		cam->viewport_width = cam->viewport_height * ((float)(cam->image_width) / cam->image_height);
		cam->camera_center = v3(0, 0, 0);

		cam->viewport_u = v3(cam->viewport_width, 0, 0);
		cam->viewport_v = v3(0, -cam->viewport_height, 0);

		cam->pixel_delta_u = v3_div_f32(cam->viewport_u, (float)cam->image_width);
		cam->pixel_delta_v = v3_div_f32(cam->viewport_v, (float)cam->image_height);

		cam->viewport_upper_left = viewport_top_left(cam->camera_center, cam->viewport_u, cam->viewport_v, cam->focal_length);

		cam->pixel00_loc = pixel00_location(cam->viewport_upper_left, cam->pixel_delta_u, cam->pixel_delta_v);
}

// #include <float.h>

// not done
// static inline
// void init_render_cam(t_camera *cam)
// {
// 	t_v3 u;
// 	t_v3 v;
// 	t_v3 w;
// 	// cam->aspect_ratio = 16.0 / 9.0;
// 	// cam->image_width = 400;
// 	cam->aspect_ratio = IMAGE_WIDTH / IMAGE_HEIGHT;
// 	cam->image_width = IMAGE_WIDTH;
// 	cam->image_height = (int32_t)(cam->image_width / cam->aspect_ratio);
// 	if (cam->image_height < 1)
// 		cam->image_height = 1;

// 	cam->focal_length = 1.0f;
// 	cam->viewport_height = 2.0f;
// 	cam->viewport_width = cam->viewport_height * ((float)(cam->image_width) / cam->image_height);
// 	cam->camera_center = v3(0, 0, 0);

// 	cam->viewport_u = v3(cam->viewport_width, 0, 0);
// 	cam->viewport_v = v3(0, -cam->viewport_height, 0);

// 	cam->pixel_delta_u = v3_div_f32(cam->viewport_u, (float)cam->image_width);
// 	cam->pixel_delta_v = v3_div_f32(cam->viewport_v, (float)cam->image_height);

// 	// t_v3 viewport_upper_left = v3_sub_v3(camera_center, (v3_sub_v3(v3_sub_v3(v3(0, 0, focal_length), v3_div_f32(viewport_u, 2.0f)), v3_div_f32(viewport_v, 2.0f))));
// 	// t_v3 viewport_upper_left = v3_sub_v3(camera_center, v3(0, 0, focal_length));
// 	// viewport_upper_left = v3_sub(viewport_upper_left, )
// 	cam->viewport_upper_left = viewport_top_left(cam->camera_center, cam->viewport_u, cam->viewport_v, cam->focal_length);

// 	// t_v3 pixel00_loc = v3_add_v3(viewport_upper_left, f32_mul_v3(0.5f, (v3_add_v3(pixel_delta_u, pixel_delta_v))));
// 	cam->pixel00_loc = pixel00_location(cam->viewport_upper_left, cam->pixel_delta_u, cam->pixel_delta_v);
// }
