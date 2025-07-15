
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

// inline
// void sphere_hit(t_sphere sphere, const t_ray *ray)
// {

// 	t_v3 relative_origin = v3_sub_v3(sphere.position, ray->origin); // or origin - center?
// 	const float a = length_sq(ray->direction);
// 	const float b = 2.0f * inner(ray->direction, relative_origin);
// 	const float c = length_sq(relative_origin) - sphere.radius * sphere.radius;
// 	const float denom = 2.0f*a;
// 	const float root_term =  square_root(b * b - 4.0f * a * c);

// 	if (root_term > TOLERANCE)
// 	{
// 		float tp = (-b + root_term) / denom;
// 		float tn = (-b - root_term) / denom;
// 		float t = tp;
// 	}
// }

inline
float sphere_hit(t_sphere sphere, const t_ray *ray)
{

	t_v3 relative_origin = v3_sub_v3(sphere.position, ray->origin); // or origin - center?
	const float a = length_sq(ray->direction);
	const float h = inner(ray->direction, relative_origin);
	const float c = length_sq(relative_origin) - sphere.radius * sphere.radius;
	const float discriminant =  h * h - a * c;

	if (discriminant < TOLERANCE)
	{
		return (-1.0f);
	}
	else
	{
		return (square_root(discriminant) / a);
	}
}

inline
int check_spheres(t_context *context, t_ray *ray)
{
	uint32_t i;
	i = 0;

	float dist;
	while (i < context->spheres->count)
	{
		dist = sphere_hit(context->spheres->arr[i], ray);
		++i;
	}
}


inline
t_v3 ray_cast(t_context *context, t_ray *ray)
{
	t_hit hit;
	t_v3  color;
	uint32_t bounce_count;

	hit = (t_hit){0, F32MAX, 0, 0};
	color = (t_v3){0};
	bounce_count = 0;

	while (bounce_count < context->cam->max_bounce)
	{

		++bounce_count;
	}


	return (color);
}


int trace(t_context *context)
{
	uint32_t y;
	uint32_t x;
	uint32_t sample_count;

	y = 0;
	while (y < context->image->height)
	{
		x = 0;
		while (x < context->image->width)
		{
			sample_count = 0;
			while (sample_count < context->cam->samples_per_pixel)
			{
				t_ray ray;


				ray.origin = context->cam->view_point;
				ray.direction = noz(v3_sub_v3(ray.origin, ));
				t_v3 color = ray_cast(context, &ray);
				++sample_count;
			}
			++x;
		}
		++y;
	}
	return (0);
}
