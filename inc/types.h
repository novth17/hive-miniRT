#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>

typedef struct u_cordinate
{
	uint32_t x;
	uint32_t y;
} t_cord;

typedef union u_vec2
{
	struct
	{
		float	x;
		float	y;
	};
	struct
	{
		float	u;
		float	v;
	};
	float	elements[2];
}	t_v2;

typedef t_v2	t_point2;

typedef union u_vec3
{
	struct
	{
		float	x;
		float	y;
		float	z;
	};
	struct
	{
		float	u;
		float	v;
		float	w;
	};
	struct
	{
		float	r;
		float	g;
		float	b;
	};
	float	elements[3];
}	t_v3;

typedef t_v3	t_vec3;
typedef t_v3	t_point3;
typedef t_v3	t_color;

typedef union u_vec4
{
	struct
	{
		union
		{
			t_v3	xyz;
			struct
			{
				float	x;
				float	y;
				float	z;
			};
		};
		float	w;
	};
	struct
	{
		union
		{
			t_v3	rgb;
			struct
			{
				float	r;
				float	g;
				float	b;
			};
		};
		float	a;
	};
	float	elements[4];
}	t_v4;


typedef union
{
	struct
	{

		t_v4 x;
		t_v4 y;
		t_v4 z;
		t_v4 w;
	};
	float mat[4][4];
} t_mat4;


typedef struct
{
	t_v3 origin;
	t_v3 direction;
} t_ray;

#endif
