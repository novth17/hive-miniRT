# ifndef OBJECT_H
# define OBJECT_H

#include "mini_rt.h"
#include "types.h"

#define SP_ARG_MIN 4
#define SP_ARG_MAX 8

#define PL_ARG_MIN 4
#define PL_ARG_MAX 8

#define CYL_ARG_MIN 6
#define CYL_ARG_MAX 10

#define FLOAT_EPSILON 1e-4f
#define SHADOW_BIAS 1e-4f
#define CYL_CAP_EPSILON 1e-4f
#define RAY_PARALLEL_EPSILON 1e-6f
#define QUADRATIC_A_EPSILON 1e-8f

typedef struct	s_light
{
	t_vec3	origin;
	float	bright_ratio;
	t_color	color;

}	t_light;

typedef enum e_obj_type
{
	LIGHT,
	SPHERE,
	PLANE,
	CYLINDER
	// later: TRIANGLE, CAT UIIA, etc.
}				t_obj_type;

typedef struct s_material
{
	t_color	color;
	float	specular_probability;
	float	diffuse;
	float	emitter;
	t_color specular_color;
} t_material;

typedef struct s_sphere
{
	t_vec3		center;
	float		radius;
	t_material	material;

}				t_sphere;

typedef struct s_plane
{
	t_vec3	point;
	t_vec3	axis;
	t_material	material;
}				t_plane;

typedef struct s_cylinder
{
	t_vec3		center;
	t_vec3		axis;
	float		diameter;
	float		height;
	t_material	material;
}				t_cylinder;

typedef struct s_object
{
	t_obj_type	type;
	union {
		t_sphere	sphere;
		t_cylinder	cyl;
		t_plane		pl;
		t_light		light;
	};
	struct s_object	*next;
}				t_object;

#endif
