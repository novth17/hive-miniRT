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

// convert from linear to srgb and pack
inline
uint32_t	exact_pack(t_v4 unpacked)
{
	const t_v4		srgb = {
		.r = exact_linear_to_srgb(unpacked.r),
		.g = exact_linear_to_srgb(unpacked.g),
		.b = exact_linear_to_srgb(unpacked.b),
		.a = unpacked.a,
	};
	const uint32_t	a = (uint32_t)(roundf(255.0f * srgb.a));
	const uint32_t	b = (uint32_t)(roundf(255.0f * srgb.b));
	const uint32_t	g = (uint32_t)(roundf(255.0f * srgb.g));
	const uint32_t	r = (uint32_t)(roundf(255.0f * srgb.r));

	return (a << 24 | b << 16 | g << 8 | r << 0);
}

// unpack and convet from srgb to linear
inline
t_v4	exact_unpack(uint32_t packed)
{
	const t_v4	unpacked = {
		.a = (float)((packed >> 24) & 0xFF) / 255.0f,
		.b = (float)((packed >> 16) & 0xFF) / 255.0f,
		.g = (float)((packed >> 8) & 0xFF) / 255.0f,
		.r = (float)((packed >> 0) & 0xFF) / 255.0f,
	};
	t_v4		result;

	result.r = exact_srgb_to_linear(unpacked.r);
	result.g = exact_srgb_to_linear(unpacked.g);
	result.b = exact_srgb_to_linear(unpacked.b);
	result.a = unpacked.a;
	return (result);
}
