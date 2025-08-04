#include "../inc/rt_math.h"
#include "../inc/mini_rt.h"

// inline
// t_v4 v4_mul_v4(t_v4 a, t_v4 b)
// {
// 	t_v4 result;

// 	result.x = a.x * b.x;
// 	result.y = a.y * b.y;
// 	result.z = a.z * b.z;
// 	result.w = a.w * b.w;
// 	return (result);
// }

t_mat4 mat_div(t_mat4 m, float scalar)
{
	m.mat[0][0] /= scalar;
	m.mat[0][1] /= scalar;
	m.mat[0][2] /= scalar;
	m.mat[0][3] /= scalar;

	m.mat[1][0] /= scalar;
	m.mat[1][1] /= scalar;
	m.mat[1][2] /= scalar;
	m.mat[1][3] /= scalar;

	m.mat[2][0] /= scalar;
	m.mat[2][1] /= scalar;
	m.mat[2][2] /= scalar;
	m.mat[2][3] /= scalar;

	m.mat[3][0] /= scalar;
	m.mat[3][1] /= scalar;
	m.mat[3][2] /= scalar;
	m.mat[3][3] /= scalar;
	return (m);
}

static inline
void set_subfactors(float subfator[18], const t_mat4 m)
{
	subfator[0] = m.mat[2][2] * m.mat[3][3] - m.mat[3][2] * m.mat[2][3];
	subfator[1] = m.mat[2][1] * m.mat[3][3] - m.mat[3][1] * m.mat[2][3];
	subfator[2] = m.mat[2][1] * m.mat[3][2] - m.mat[3][1] * m.mat[2][2];
	subfator[3] = m.mat[2][0] * m.mat[3][3] - m.mat[3][0] * m.mat[2][3];
	subfator[4] = m.mat[2][0] * m.mat[3][2] - m.mat[3][0] * m.mat[2][2];
	subfator[5] = m.mat[2][0] * m.mat[3][1] - m.mat[3][0] * m.mat[2][1];
	subfator[6] = m.mat[1][2] * m.mat[3][3] - m.mat[3][2] * m.mat[1][3];
	subfator[7] = m.mat[1][1] * m.mat[3][3] - m.mat[3][1] * m.mat[1][3];
	subfator[8] = m.mat[1][1] * m.mat[3][2] - m.mat[3][1] * m.mat[1][2];
	subfator[9] = m.mat[1][0] * m.mat[3][3] - m.mat[3][0] * m.mat[1][3];
	subfator[10] = m.mat[1][0] * m.mat[3][2] - m.mat[3][0] * m.mat[1][2];
	subfator[11] = m.mat[1][0] * m.mat[3][1] - m.mat[3][0] * m.mat[1][1];
	subfator[12] = m.mat[1][2] * m.mat[2][3] - m.mat[2][2] * m.mat[1][3];
	subfator[13] = m.mat[1][1] * m.mat[2][3] - m.mat[2][1] * m.mat[1][3];
	subfator[14] = m.mat[1][1] * m.mat[2][2] - m.mat[2][1] * m.mat[1][2];
	subfator[15] = m.mat[1][0] * m.mat[2][3] - m.mat[2][0] * m.mat[1][3];
	subfator[16] = m.mat[1][0] * m.mat[2][2] - m.mat[2][0] * m.mat[1][2];
	subfator[17] = m.mat[1][0] * m.mat[2][1] - m.mat[2][0] * m.mat[1][1];
};


t_mat4 inverse(t_mat4 m)
{
	float determinant;
	float subfactor[18];

	set_subfactors(subfactor, m);

	t_mat4 result = {};
	result.mat[0][0] = + (m.mat[1][1] * subfactor[0] - m.mat[1][2] * subfactor[1] + m.mat[1][3] * subfactor[2]);
	result.mat[0][1] = - (m.mat[1][0] * subfactor[0] - m.mat[1][2] * subfactor[3] + m.mat[1][3] * subfactor[4]);
	result.mat[0][2] = + (m.mat[1][0] * subfactor[1] - m.mat[1][1] * subfactor[3] + m.mat[1][3] * subfactor[5]);
	result.mat[0][3] = - (m.mat[1][0] * subfactor[2] - m.mat[1][1] * subfactor[4] + m.mat[1][2] * subfactor[5]);

	result.mat[1][0] = - (m.mat[0][1] * subfactor[0] - m.mat[0][2] * subfactor[1] + m.mat[0][3] * subfactor[2]);
	result.mat[1][1] = + (m.mat[0][0] * subfactor[0] - m.mat[0][2] * subfactor[3] + m.mat[0][3] * subfactor[4]);
	result.mat[1][2] = - (m.mat[0][0] * subfactor[1] - m.mat[0][1] * subfactor[3] + m.mat[0][3] * subfactor[5]);
	result.mat[1][3] = + (m.mat[0][0] * subfactor[2] - m.mat[0][1] * subfactor[4] + m.mat[0][2] * subfactor[5]);

	result.mat[2][0] = + (m.mat[0][1] * subfactor[6] - m.mat[0][2] * subfactor[7] + m.mat[0][3] * subfactor[8]);
	result.mat[2][1] = - (m.mat[0][0] * subfactor[6] - m.mat[0][2] * subfactor[9] + m.mat[0][3] * subfactor[10]);
	result.mat[2][2] = + (m.mat[0][0] * subfactor[7] - m.mat[0][1] * subfactor[9] + m.mat[0][3] * subfactor[11]);
	result.mat[2][3] = - (m.mat[0][0] * subfactor[8] - m.mat[0][1] * subfactor[10] + m.mat[0][2] * subfactor[11]);

	result.mat[3][0] = - (m.mat[0][1] * subfactor[12] - m.mat[0][2] * subfactor[13] + m.mat[0][3] * subfactor[14]);
	result.mat[3][1] = + (m.mat[0][0] * subfactor[12] - m.mat[0][2] * subfactor[15] + m.mat[0][3] * subfactor[16]);
	result.mat[3][2] = - (m.mat[0][0] * subfactor[13] - m.mat[0][1] * subfactor[15] + m.mat[0][3] * subfactor[17]);
	result.mat[3][3] = + (m.mat[0][0] * subfactor[14] - m.mat[0][1] * subfactor[16] + m.mat[0][2] * subfactor[17]);

	determinant =
		+ m.mat[0][0] * result.mat[0][0]
		+ m.mat[0][1] * result.mat[0][1]
		+ m.mat[0][2] * result.mat[0][2]
		+ m.mat[0][3] * result.mat[0][3];

	result = mat_div(result, determinant);
	return (result);
}

t_mat4 look_at(const t_camera *restrict cam)
{
	const t_v3 f = cam->lookat;
	const t_v3 s = unit_vector(cross(f, cam->vup));
	const t_v3 u = cross(s, f);


    t_mat4 result;
    result.mat[0][0] = s.x;
    result.mat[1][0] = s.y;
    result.mat[2][0] = s.z;
    result.mat[0][1] = u.x;
    result.mat[1][1] = u.y;
    result.mat[2][1] = u.z;
    result.mat[0][2] =-f.x;
    result.mat[1][2] =-f.y;
    result.mat[2][2] =-f.z;
    result.mat[3][0] =-dot(s, cam->lookfrom);
    result.mat[3][1] =-dot(u, cam->lookfrom);
    result.mat[3][2] = dot(f, cam->lookfrom);
	result.mat[0][3] = 1;
	result.mat[1][3] = 1;
	result.mat[2][3] = 1;
	result.mat[3][3] = 1;
    return (result);
}


float deg_to_rad(float a)
{
	return (a * M_PI / 180);
}

t_mat4 perspective_fov(float fov, float width, float height, float near, float far)
{
	const float rad = fov;
	const float h = cosf(0.5f * rad) / sinf(0.5f * rad);
	const float w = h * height / width;
	t_mat4 result;

	ft_memset(&result, 0, sizeof(result));
	result.mat[0][0] = w;
	result.mat[1][1] = h;
	result.mat[2][2] = far / (near - far);
	result.mat[2][3] = -1.0f;
	result.mat[3][2] = -(far * near) / (far - near);
	return (result);
}

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



// to set defaults
void base_init_cam(t_camera *cam)
{

	cam->samples_per_pixel = 1;
	cam->pixel_sample_scale =  1.0f / cam->samples_per_pixel;
	cam->sqrt_spp = (int32_t)square_root(cam->samples_per_pixel);
	cam->pixel_sample_scale_strati = 1.0 / (cam->sqrt_spp * cam->sqrt_spp);
	cam->recip_sqrt_spp = 1.0 / cam->sqrt_spp;

	cam->max_bounce = 2;
	cam->vup = v3(0, 1, 0); // might not need this in camera

	cam->defocus_angle = 0.0f;
	cam->focus_dist = 1.0;

}

void init_camera_for_frame(t_minirt *minirt, t_camera *cam)
{
	const t_v3 w = neg(cam->lookat);
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


	// t_mat4 projection;
	//https://www.songho.ca/opengl/gl_projectionmatrix.html
	cam->projection = perspective_fov(deg_to_rad(cam->fov), (float)cam->image_width, (float)cam->image_height, MIN_HIT_DIST, MAX_HIT_DIST);
	cam->inverse_projection = inverse(cam->projection);
	cam->view = look_at(cam);
	cam->inverse_view = inverse(cam->view);

}
