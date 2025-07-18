
#include "../test_stuff/rt_math.h"
#include <ctype.h>
#include <inttypes.h>
#include <math.h>
#include <stdint.h>

#include "MLX42.h"

// these are placeholder limits for now
// should look at what numbers make sense later
#define MIN_HIT_DIST 0.001f
#define TOLERANCE	 0.0001f

/*

typedef struct
{
	float		aspect_ratio;
	uint32_t	samples_per_pixel; // how many rays per pixel
	uint32_t	max_bounce; // 1 or 2 for mandatory
	t_v3		view_point; // where we are looking from
	t_v3		direction; // where we are looking towards;
	float		fov; //
} t_camera;


typedef struct s_image
{
	uint32_t width;
	uint32_t height;
	uint32_t *pixels;
} t_image;

typedef struct plane
{
	t_v3	normal;
	float	d;
	t_v3 	color;
} t_plane;

typedef struct
{
	t_plane *arr;
	uint32_t count;
} t_planes;

typedef struct
{
	t_v3	position;
	float	radius;
	t_v3 	color;
} t_sphere;

typedef struct
{
	t_sphere *arr;
	uint32_t count;
} t_spheres;

typedef struct
{
	t_v3 a;
	t_v3 b;
	float radius;
	//@TODO implement;
} t_cylinder;

typedef struct
{
	t_cylinder *arr;
	uint32_t count;
} t_cylinders;

typedef struct s_context
{
	t_camera	*cam;
	t_planes	*planes;
	t_spheres	*spheres;
	t_cylinders	*cylinders;
	mlx_image_t *image;
} t_context;

typedef struct
{
	bool  did_hit;
	float distance;
	t_v3  point;
	t_v3  normal;
} t_hit;

typedef struct
{
	t_v3 origin;
	t_v3 direction;
} t_ray;

*/

// everything under this is definetely going to be re done somewhere else

static float ExactLinearTosRGB(float L);


typedef struct
{
	float	aspect_ratio; //
	int32_t image_width;
	int32_t image_height;

	float focal_length;
	float viewport_height;
	float viewport_width;
	t_v3 camera_center;

	t_v3 viewport_u;
	t_v3 viewport_v;

	t_v3 pixel_delta_u;
	t_v3 pixel_delta_v;

	t_v3 viewport_upper_left;
	t_v3 pixel00_location;
} t_camera;

#define IMAGE_WIDTH 1280.0f
#define IMAGE_HEIGHT 720.0f


int main(void)
{

	float	aspect_ratio = IMAGE_WIDTH / IMAGE_HEIGHT;
	int32_t image_width = IMAGE_WIDTH;
	int32_t image_height = (int32_t)(image_width / aspect_ratio);
	if (image_height < 1)
		image_height = 1;

	float focal_length = 1.0f;
	float viewport_height = 2.0f;
	float viewport_width = viewport_height * (float)(image_width) / image_height;

	t_v3 viewport_u = v3(viewport_width, 0, 0);
	t_v3 viewport_v = v3(0, -viewport_height, 0);

	t_v3 pixel_delta_u = v3_div_f32(viewport_u, (float)image_width);
	t_v3 pixel_delta_v = v3_div_f32(viewport_v, (float));

	t_v3 viewport_upper_left;
	t_v3 pixel00_location;
}

static
float ExactLinearTosRGB(float L)
{
	float S;

	if (L < 0.0f)
	{
		L = 0.0f;
	}

	if (L > 1.0f)
	{
		L = 1.0f;
	}

	S = L * 12.92;
	if (L > 0.0031308)
	{
		S = 1.055F*pow(L, 1.0f/2.4f) - 0.055f;
	}
	return (S);
}
