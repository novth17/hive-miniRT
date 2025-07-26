#include "../inc/rt_math.h"
#include "../inc/mini_rt.h"
#include <stdint.h>
#include <math.h> // check

// typedef struct
// {
// 	float	aspect_ratio; //
// 	int32_t image_width;
// 	int32_t image_height;
// 	int32_t samples_per_pixel;
// 	int32_t max_bounce; // for mandatory 1 or 2 i guess;

// 	float		fov;
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
t_v3	viewport_top_left(const t_camera *cam, const t_v3 w)
{
	t_v3	result;

	result = V3_SUB(cam->camera_center, f32_mul_v3(cam->focus_dist, w));
	result = V3_SUB(result, v3_div_f32(cam->viewport_u, 2.0f));
	result = V3_SUB(result, v3_div_f32(cam->viewport_v, 2.0f));
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
	ft_memset(cam, 0, sizeof(*cam));

	cam->samples_per_pixel = 8;
	cam->max_bounce = 1;

	cam->fov = 90.0f;
	cam->lookfrom = v3(0, 0, 0);
	cam->lookat = v3(0, 0, -1);
	cam->vup = v3(0, 1, 0); // might not need this in camera

	cam->defocus_angle = 2;
	cam->focus_dist = 1.0;

}

bool init_camera_for_frame(t_minirt *minirt, t_camera *cam)
{
	// static const t_v3 vup = {0, 1, 0};
	const t_v3 w = unit_vector(V3_SUB(cam->lookfrom, cam->lookat));
	const t_v3 u = unit_vector(cross(cam->vup, w));
	const t_v3 v = cross(w, u);
	const float h = (float)tan((cam->fov * M_PI / 180) / 2);
	// cam->aspect_ratio = 16.0 / 9.0;
	// cam->image_width = 400;
	cam->pixel_sample_scale = 1.0f / cam->samples_per_pixel;
	cam->aspect_ratio = (float)minirt->image->width / minirt->image->height;
	cam->image_width = minirt->image->width;
	cam->image_height = (int32_t)(cam->image_width / cam->aspect_ratio);
	if (cam->image_height < 1)
		cam->image_height = 1;


	// cam->focal_length = length(V3_SUB(cam->lookfrom, cam->lookat));
	cam->viewport_height = 2.0f * h * cam->focus_dist;
	cam->viewport_width = cam->viewport_height * ((float)(cam->image_width) / cam->image_height);
	cam->camera_center = cam->lookfrom;

	cam->viewport_u = f32_mul_v3(cam->viewport_width, u);
	cam->viewport_v = f32_mul_v3(cam->viewport_height, neg(v));

	cam->pixel_delta_u = v3_div_f32(cam->viewport_u, (float)cam->image_width);
	cam->pixel_delta_v = v3_div_f32(cam->viewport_v, (float)cam->image_height);

	cam->viewport_upper_left = viewport_top_left(cam, w);

 	const float defocus_radius = cam->focus_dist * tan((cam->defocus_angle / 2) * M_PI / 180);
  	cam->defocus_disk_u = v3_mul_f32(u, defocus_radius);
    cam->defocus_disk_v = v3_mul_f32(v, defocus_radius);

	cam->pixel00_loc = pixel00_location(cam->viewport_upper_left, cam->pixel_delta_u, cam->pixel_delta_v);
	return (false);
}

// void init_camera_for_frame(t_camera *cam)
// {
// 	// ft_memmove(cam, base_cam, sizeof(*cam));
// 	// cam->aspect_ratio = (float)minirt->image->width / minirt->image->height;
// 	// cam->image_width = minirt->image->width;
// 	// cam->image_height = (int32_t)(cam->image_width / cam->aspect_ratio);
// 	// if (cam->image_height < 1)
// 	// 	cam->image_height = 1;

// 	// cam->samples_per_pixel = 10;


// 	// cam->focal_length = 1.0f;
// 	// cam->camera_center = v3(0, 0, 0);
// 	// cam->viewport_height = 2.0f;

// 	cam->viewport_width = cam->viewport_height * ((float)(cam->image_width) / cam->image_height);

// 	cam->viewport_u = v3(cam->viewport_width, 0, 0);
// 	cam->viewport_v = v3(0, -cam->viewport_height, 0);

// 	cam->pixel_delta_u = v3_div_f32(cam->viewport_u, (float)cam->image_width);
// 	cam->pixel_delta_v = v3_div_f32(cam->viewport_v, (float)cam->image_height);

// 	cam->viewport_upper_left = viewport_top_left(cam->camera_center, cam->viewport_u, cam->viewport_v, cam->focal_length);

// 	cam->pixel00_loc = pixel00_location(cam->viewport_upper_left, cam->pixel_delta_u, cam->pixel_delta_v);
// }


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
