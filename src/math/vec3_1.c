#include "rt_math.h"

inline
t_v3	v3(float x, float y, float z)
{
	return ((t_v3){.x = x, .y = y, .z = z});
}

inline
t_v3	neg(t_v3 a)
{
	t_v3	result;

	result.x = -a.x;
	result.y = -a.y;
	result.z = -a.z;
	return (result);
}

inline
t_v3	v3_add_v3(t_v3 a, t_v3 b)
{
	t_v3	result;

	result.x = a.x + b.x;
	result.y = a.y + b.y;
	result.z = a.z + b.z;
	return (result);
}

inline
t_v3	v3_add_f32(t_v3 a, float b)
{
	t_v3	result;

	result.x = a.x + b;
	result.y = a.y + b;
	result.z = a.z + b;
	return (result);
}

inline
t_v3	v3_sub_v3(t_v3 a, t_v3 b)
{
	t_v3	result;

	result.x = a.x - b.x;
	result.y = a.y - b.y;
	result.z = a.z - b.z;
	return (result);
}
