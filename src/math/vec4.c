#include "rt_math.h"

inline
t_v4	v4(float x, float y, float z, float w)
{
	return ((t_v4){.x = x, .y = y, .z = z, .w = w});
}

inline
t_v4	v4_mul_f32(t_v4 a, float b)
{
	t_v4	result;

	result.x = a.x * b;
	result.y = a.y * b;
	result.z = a.z * b;
	result.w = a.w * b;
	return (result);
}

inline
t_v4	v4_add(t_v4 a, t_v4 b)
{
	t_v4	result;

	result.x = a.x + b.x;
	result.y = a.y + b.y;
	result.z = a.z + b.z;
	result.w = a.w + b.w;
	return (result);
}

inline
t_v4	v3_to_v4(t_v3 v3, float w)
{
	t_v4	result;

	result.x = v3.x;
	result.y = v3.y;
	result.z = v3.z;
	result.w = w;
	return (result);
}
