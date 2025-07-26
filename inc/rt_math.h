#ifndef RT_MATH_H
# define RT_MATH_H

# include <stdint.h>
# include <float.h>
// # include <math.h> // @TODO watch out for this

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

# define U16MAX	65535
# define U32MAX	UINT32_MAX // ((uint32_t)-1)
# define U64MAX UINT64_MAX // ((uint64_t)-1)
# define F32MAX	FLT_MAX
# define F32MIN	FLT_MIN

t_v2		v2(float x, float y);
t_v2		v2s(int32_t x, int32_t y);
t_v2		v2u(uint32_t x, uint32_t y);

t_v3		v3(float x, float y, float z); // create a vec3
t_v3		v3_add_v3(t_v3 a, t_v3 b); // vec3 a + vec3 b
# define V3_ADD v3_add_v3

t_v3		v3_add_f32(t_v3 a, float b); // vec3 a + float b
t_v3		v3_sub_v3(t_v3 a, t_v3 b); // vec3 a - vec3 b
# define V3_SUB v3_sub_v3

t_v3		v3_mul_v3(t_v3 a, t_v3 b); // vec3 a * vec3 b
t_v3		v3_mul_f32(t_v3 a, float b);
t_v3		f32_mul_v3(float a, t_v3 b); // float a * vec3 b;

t_v3		f32_div_v3(float a, t_v3 b);
t_v3		v3_div_f32(t_v3 a, float b);

t_v4		v4(float x, float y, float z, float w);
t_v4		linear_to_srgb255(t_v4 c);

uint32_t	rgba_pack(t_v4 unpacked);
t_v4		rgba_unpack(uint32_t packed);
uint32_t	rgba_pack4x8(t_v4 unpacked);

float		lerp(float a, float t, float b);
t_v4		v4_lerp(t_v4 a, float t, t_v4 b);
t_v3		v3_lerp(t_v3 a, float t, t_v3 b);

// these are the same operation
float		inner(t_v3 a, t_v3 b);
float		dot(t_v3 a, t_v3 b);

t_v3		cross(t_v3 a, t_v3 b);

float		square_root(float a);
float		square(float a);
float		length_sq(t_v3 a);
float		length(t_v3 a);

t_v3		unit_vector(t_v3 a);
t_v3		normalize(t_v3 a);
t_v3		noz(t_v3 a);

float		sign(float value);
t_v3		neg(t_v3 a);

t_v3		hadamard(t_v3 a, t_v3 b); // hadamard attennuation

#endif
