#include "rt_math.h"

inline
t_v3	cross(t_v3 a, t_v3 b)
{
	t_v3	result;

	result.x = (a.y * b.z) - (a.z * b.y);
	result.y = (a.z * b.x) - (a.x * b.z);
	result.z = (a.x * b.y) - (a.y * b.x);
	return (result);
}

inline
float	dot(t_v3 a, t_v3 b)
{
	float	result;

	result = (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
	return (result);
}

inline
float	length(t_v3 a)
{
	float	result;

	result = square_root(length_sq(a));
	return (result);
}
