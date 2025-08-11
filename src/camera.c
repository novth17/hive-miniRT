#include "../inc/rt_math.h"
#include "../inc/mini_rt.h"

float deg_to_rad(float a)
{
	return (a * M_PI / 180);
}

inline
t_v3	viewport_top_left(const t_camera *cam, const t_v3 w)
{
	t_v3	result;

	result = V3_SUB(cam->camera_center, f32_mul_v3(cam->focus_dist, w));
	result = V3_SUB(result, v3_div_f32(cam->viewport_u, 2.0f));
	result = V3_SUB(result, v3_div_f32(cam->viewport_v, 2.0f));
	return (result);
}

inline
t_v3	pixel00_location(const t_v3 viewport_upper_left, const t_v3 pixel_delta_u, const t_v3 pixel_delta_v)
{
	t_v3	result;

	result = v3_add_v3(pixel_delta_u, pixel_delta_v);
	result = f32_mul_v3(0.5, result);
	result = v3_add_v3(viewport_upper_left, result);
	return (result);
}

// to set defaults
void base_init_cam(t_camera *cam)
{

	cam->samples_per_pixel = 1;
	cam->pixel_sample_scale = 1.0f / cam->samples_per_pixel;
	cam->sqrt_spp = (int32_t)square_root(cam->samples_per_pixel);
	cam->pixel_sample_scale_strati = 1.0 / (cam->sqrt_spp * cam->sqrt_spp);
	cam->recip_sqrt_spp = 1.0 / cam->sqrt_spp;

	cam->max_bounce = 4;
	cam->vup = v3(0, 1, 0); // might not need this in camera

	cam->defocus_angle = 0.0f;
	cam->focus_dist = 1.0;

}

void init_camera_for_frame(t_minirt *minirt, t_camera *cam)
{
	const t_v3 w = cam->lookat;
	const t_v3 u = unit_vector(cross(cam->vup, w));
	const t_v3 v = cross(w, u);
	const float h = tanf(deg_to_rad(cam->fov) * 0.5);
	const float defocus_radius = cam->focus_dist * tanf(deg_to_rad(cam->defocus_angle * 0.5));


	// cam->pixel_sample_scale = 1.0f / cam->samples_per_pixel;
	cam->aspect_ratio = (float)minirt->image->width / minirt->image->height;
	cam->image_width = minirt->image->width;
	cam->image_height = (int32_t)(cam->image_width / cam->aspect_ratio);
	if (cam->image_height < 1)
		cam->image_height = 1;
	cam->right = u;


	// cam->focal_length = length(V3_SUB(cam->lookfrom, cam->lookat));
	cam->viewport_height = 2.0f * h * cam->focus_dist;
	cam->viewport_width = cam->viewport_height * ((float)(cam->image_width) / cam->image_height);
	cam->camera_center = cam->lookfrom;

	cam->viewport_u = f32_mul_v3(cam->viewport_width, u); // right
	cam->viewport_v = f32_mul_v3(cam->viewport_height, neg(v)); // up

	cam->pixel_delta_u = v3_div_f32(cam->viewport_u, (float)cam->image_width);
	cam->pixel_delta_v = v3_div_f32(cam->viewport_v, (float)cam->image_height);

	cam->viewport_upper_left = viewport_top_left(cam, w);

  	cam->defocus_disk_u = v3_mul_f32(u, defocus_radius);
    cam->defocus_disk_v = v3_mul_f32(v, defocus_radius);

	cam->pixel00_loc = pixel00_location(cam->viewport_upper_left, cam->pixel_delta_u, cam->pixel_delta_v);

}
