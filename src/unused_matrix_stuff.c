#include "../inc/rt_math.h"
#include "../inc/mini_rt.h"


// t_v4		mat_mul_v4(t_mat4 m, t_v4 v);


#if 0
inline
t_v4 mat_mul_v4(t_mat4 m, t_v4 v)
{
	t_v4 result;

	result.x = v.x * m.mat[0][0] + v.y * m.mat[0][1] + v.z * m.mat[0][2] + v.w * m.mat[0][3];
	result.y = v.x * m.mat[1][0] + v.y * m.mat[1][1] + v.z * m.mat[1][2] + v.w * m.mat[1][3];
	result.z = v.x * m.mat[2][0] + v.y * m.mat[2][1] + v.z * m.mat[2][2] + v.w * m.mat[2][3];
	result.w = v.x * m.mat[3][0] + v.y * m.mat[3][1] + v.z * m.mat[3][2] + v.w * m.mat[3][3];
	return (result);
}
#else
inline
t_v4 mat_mul_v4(t_mat4 m, t_v4 v)
{
	t_v4 result;

	result.x = v.x * m.mat[0][0] + v.y * m.mat[1][0] + v.z * m.mat[2][0] + v.w * m.mat[3][0];
	result.y = v.x * m.mat[0][1] + v.y * m.mat[1][1] + v.z * m.mat[2][1] + v.w * m.mat[3][1];
	result.z = v.x * m.mat[0][2] + v.y * m.mat[1][2] + v.z * m.mat[2][2] + v.w * m.mat[3][2];
	result.w = v.x * m.mat[0][3] + v.y * m.mat[1][3] + v.z * m.mat[2][3] + v.w * m.mat[3][3];
	return (result);
}
#endif

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


// just to remember stuff
// not a real function
void func(t_camera *cam)
{

		// t_mat4 projection;
	//https://www.songho.ca/opengl/gl_projectionmatrix.html
	cam->projection = perspective_fov(deg_to_rad(cam->fov), (float)cam->image_width, (float)cam->image_height, MIN_HIT_DIST, MAX_HIT_DIST);
	cam->inverse_projection = inverse(cam->projection);
	cam->view = look_at(cam);
	cam->inverse_view = inverse(cam->view);
}
