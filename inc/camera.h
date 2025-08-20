#ifndef CAMERA_H
#define CAMERA_H

#include "rt_math.h"
#include <stdint.h>

typedef struct s_camera
{
	t_v3 base_direction;
	float	aspect_ratio; //
	int32_t image_width;
	int32_t image_height;

	int32_t samples_per_pixel;
	float	pixel_sample_scale;

	float pixel_sample_scale_strati;
	int32_t	sqrt_spp;
	float	recip_sqrt_spp;


	int32_t max_bounce; // for mandatory probably 0

	float		fov;
	t_point3	lookfrom; // @QUESTION why this and camera_center??
	t_point3	lookat;
	t_v3		vup;
	t_v3		right;


 	float defocus_angle;  // Variation angle of rays through each pixel
    float focus_dist;

	t_v3 defocus_disk_u;
	t_v3 defocus_disk_v;



	// float focal_length;
	float viewport_height;
	float viewport_width;
	t_v3 camera_center;

	t_v3 viewport_u;
	t_v3 viewport_v;

	t_v3 pixel_delta_u;
	t_v3 pixel_delta_v;

	t_v3 viewport_upper_left;
	t_v3 pixel00_loc;

} t_camera;

#endif
