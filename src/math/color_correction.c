#include "rt_math.h"

// convert srgb color to linear color space
inline
float	exact_srgb_to_linear(float s)
{
	float	l;

	if (s != s)
		s = 0.0f;
	if (s < 0.0f)
	{
		s = 0.0f;
	}
	if (s > 1.0f)
	{
		s = 1.0f;
	}
	l = s * 0.0773993808;
	if (s >= 0.04045)
	{
		l = powf(s * 0.9478672986 + 0.0521327014, 2.4);
	}
	return (l);
}

// convert linear color to srgb color space
inline
float	exact_linear_to_srgb(float l)
{
	float	s;

	if (l != l)
	{
		l = 0.0f;
	}
	if (l < 0.0f)
	{
		l = 0.0f;
	}
	if (l > 1.0f)
	{
		l = 1.0f;
	}
	s = l * 12.92;
	if (l >= 0.0031308)
	{
		s = 1.055F * powf(l, 1.0f / 2.4f) - 0.055f;
	}
	return (s);
}
