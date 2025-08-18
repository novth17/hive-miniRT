#include "../../inc/rt_math.h"


// @TODO delete probably
// inline
// float Pow(float a, float b)
// {
// 	float result = (float)pow(a, b);
// 	return (result);
// }

inline // @TODO move this
t_v3 at(const t_ray r, const float t)
{
	t_v3 result;

	result = V3_ADD(r.origin, f32_mul_v3(t, r.direction));
	return (result);
}

inline
float square(float a)
{
	float result;

	result = a * a;
	return (result);
}

inline
float length_sq(t_v3 a)
{
	float result;

	result = dot(a, a);
	return (result);
}

#include <math.h>
// @TODO remake mby
inline
float square_root(float a)
{
	float result = sqrtf(a);

	return (result);
}

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
float sign(float value)
{
	if (value < 0.0)
		return (-1.0f);
	else if (value > 0.0)
		return (1.0f);
	else
		return (0.0f);
}

// maybe this relates to hue somehow

// static inline
// float saturate_branchless(float f)
// {
// 	float result = lerp(0.0f, 1.0f, f);
// 	return (result);
// }

// static inline
// uint8_t float_to_u8_sat(float f)
// {
// 	return ((saturate(f)) * 255.0f + 0.5f); // + 0.5f ?
// }

static inline
uint32_t float_to_u32_sat(float f)
{
	return ((clamp(f, 0.f, 1.f)) * 255.0f + 0.5f); // + 0.5f ?
}


inline
t_v4 linear_to_srgb255(t_v4 c)
{
	const float one255 = 255.0f;
	t_v4 result;

	result.r = one255 * square_root(c.r);
	result.g = one255 * square_root(c.g);
	result.b = one255 * square_root(c.b);
	result.a = one255 * square_root(c.a);
	return (result);
}

// inline
// t_v4 sRGB_linearize(float R, float G, float b, float a)
// {
// 	t_v4 input =
// }

// uint32_t from_linear(t_v4 unpacked)
// {

// }


// inline
// uint32_t rgba_pack(t_v4 unpacked)
// {
// 	uint32_t a = (uint32_t)(unpacked.a * 255.999);
// 	uint32_t r = (uint32_t)(unpacked.r * 255.999);
// 	uint32_t g = (uint32_t)(unpacked.g * 255.999);
// 	uint32_t b = (uint32_t)(unpacked.b * 255.999);
// 	return (a << 24 | r << 16 | g << 8 | b);
// }


inline
uint32_t rgba_pack(t_v4 unpacked)
{
	uint32_t result;

	result = ((uint32_t)(float_to_u32_sat(unpacked.a)) << 24)	|
			((uint32_t)(float_to_u32_sat(unpacked.r)) << 16)	|
			((uint32_t)(float_to_u32_sat(unpacked.g)) << 8)		|
			((uint32_t)(float_to_u32_sat(unpacked.b)) << 0);
	return (result);
}


inline
uint32_t rgba_pack4x8(t_v4 unpacked)
{
	uint32_t result;

	result = ((uint32_t)((unpacked.a + 0.5f)) << 24)	|
			((uint32_t)((unpacked.r + 0.5f)) << 16)		|
			((uint32_t)((unpacked.g + 0.5f)) << 8)		|
			((uint32_t)((unpacked.b + 0.5f)) << 0);
	return (result);
}

// TODO(luka) make this
// inline
// uint32_t rgba_pack(t_v4 unpacked)
// {
// 	uint32_t result =   {((uint32_t)(float_to_u8_sat(unpacked.a)) << 24)	|
// 					((uint32_t)(float_to_u8_sat(unpacked.r)) << 16)	|
// 					((uint32_t)(float_to_u8_sat(unpacked.g)) << 8)		|
// 					((uint32_t)(float_to_u8_sat(unpacked.b)) << 0)};
// 	return (result);
// }

inline
t_v4 rgba_unpack(uint32_t packed)
{
	t_v4 result;

	result = v4((float)((packed >> 0) & 0xFF),
					(float)((packed >> 8) & 0xFF),
					(float)((packed >> 16) & 0xFF),
					(float)((packed >> 24) & 0xFF));
	return (result);
}

inline
float dot(t_v3 a, t_v3 b)
{
	float result;

	result = a.x*b.x + a.y*b.y + a.z*b.z;
	return (result);
}

inline
t_v3 cross(t_v3 a, t_v3 b)
{
	t_v3 result;

	result.x = a.y*b.z - a.z*b.y;
	result.y = a.z*b.x - a.x*b.z;
	result.z = a.x*b.y - a.y*b.x;
	return (result);
}


inline
float length(t_v3 a)
{
	float result;

	result = square_root(length_sq(a));
	return (result);
}

inline
t_v3 hadamard(t_v3 a, t_v3 b)
{
	t_v3 result;

	result = v3(a.x*b.x, a.y*b.y, a.z*b.z);
	return (result);
}


inline
float clamp(const float f, const float min, const float max)
{
	if (f < min)
		return (min);
	else if (f > max)
		return (max);
	else
		return (f);
}

inline
t_v3 v3_clamp(const t_v3 a)
{
	t_v3 result;

	result.x = clamp(a.x, 0, 1);
	result.y = clamp(a.y, 0, 1);
	result.z = clamp(a.z, 0, 1);
	return (result);
}

inline
uint32_t rgb_pack4x8(t_v3 unpacked)
{
	uint32_t result;


	result = ((uint32_t)(0xFF) << 24)					|
			((uint32_t)((unpacked.b * 255.0f) + 0.5f) << 16)	|
			((uint32_t)((unpacked.g * 255.0f) + 0.5f) << 8)	|
			((uint32_t)((unpacked.r * 255.0f) + 0.5f) << 0);
	return (result);
}


// convert srgb color to linear color space
inline
float exact_srgb_to_linear(float srgb)
{
    float l;

	if (srgb != srgb)
		srgb = 0.0f;
    if (srgb < 0.0f)
    {
	    srgb = 0.0f;
    }
    if (srgb > 1.0f)
    {
	    srgb = 1.0f;
    }
    if (srgb < 0.04045)
	{
		l = srgb * 0.0773993808;
	}
	else
    {
	    l = powf(srgb * 0.9478672986 + 0.0521327014, 2.4);
    }
    return (l);
}


// convert linear color to srgb color space
inline
float exact_linear_to_srgb(float l)
{
    float s;

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
    if (l < 0.0031308)
	{
		s = l * 12.92;
	}
	else
    {
	    s = 1.055F * powf(l, 1.0f / 2.4f) - 0.055f;
    }
    return (s);
}

t_v3	exact_v3_srgb_to_linear(t_v3 srgb)
{
	t_v3	linear;

	linear.r = exact_srgb_to_linear(srgb.r);
	linear.g = exact_srgb_to_linear(srgb.g);
	linear.b = exact_srgb_to_linear(srgb.b);
	return (linear);
}

inline
uint32_t	exact_pack(t_v4 unpacked)
{
    const t_v4	srgb =
	{
		.r = exact_linear_to_srgb(unpacked.r),
		.g = exact_linear_to_srgb(unpacked.g),
        .b = exact_linear_to_srgb(unpacked.b),
		.a = unpacked.a,
	};
	// const uint32_t a = (uint32_t)((255.0f * srgb.a) + 0.5f);
	// const uint32_t b = (uint32_t)((255.0f * srgb.b) + 0.5f);
	// const uint32_t g = (uint32_t)((255.0f * srgb.g) + 0.5f);
	// const uint32_t r = (uint32_t)((255.0f * srgb.r) + 0.5f);
	const uint32_t a = (uint32_t)(roundf(255.0f * srgb.a));
	const uint32_t b = (uint32_t)(roundf(255.0f * srgb.b));
	const uint32_t g = (uint32_t)(roundf(255.0f * srgb.g));
	const uint32_t r = (uint32_t)(roundf(255.0f * srgb.r));

	return (a << 24 | b << 16 | g << 8 | r << 0);
}

inline
t_v4	rgb_u32_to_v4(uint32_t c)
{
	t_v4	result;

	result.a = (float)((c >> 24) & 0xFF) / 255.0f;
	result.b = (float)((c >> 16) & 0xFF) / 255.0f;
	result.g = (float)((c >> 8) & 0xFF) / 255.0f;
	result.r = (float)((c >> 0) & 0xFF) / 255.0f;
	return (result);
}

inline
t_v4 exact_unpack(uint32_t packed)
{
    const t_v4 unpacked = rgb_u32_to_v4(packed);
	t_v4 result;

	result.r = exact_srgb_to_linear(unpacked.r);
	result.g = exact_srgb_to_linear(unpacked.g);
	result.b = exact_srgb_to_linear(unpacked.b);
	result.a = unpacked.a;
	return (result);
}



inline
float gamma2_srgb_to_linear(float srgb)
{
    float l;

	if (srgb != srgb)
		srgb = 0.0f;
    if (srgb < 0.0f)
    {
	    srgb = 0.0f;
    }
    if (srgb > 1.0f)
    {
	    srgb = 1.0f;
    }
    if (srgb < 0.04045)
	{
		l = srgb * 0.0773993808;
	}
	else
    {
    	srgb = srgb * 0.9478672986 + 0.0521327014;
	    l = srgb * srgb;
    }
    return (l);
}


// convert linear color to srgb color space
inline
float gamma2_linear_to_srgb(float l)
{
    float s;

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
    if (l < 0.0031308)
	{
		s = l * 12.92;
	}
	else
    {
	    s = 1.055F * square_root(l) - 0.055f;
    }
    return (s);
}

inline
uint32_t	gamma2_pack(t_v4 unpacked)
{
    const t_v4	srgb =
	{
		.g = gamma2_linear_to_srgb(unpacked.g),
		.r = gamma2_linear_to_srgb(unpacked.r),
        .b = gamma2_linear_to_srgb(unpacked.b),
		.a = unpacked.a,
	};
	// const uint32_t a = (uint32_t)((255.0f * srgb.a) + 0.5f);
	// const uint32_t b = (uint32_t)((255.0f * srgb.b) + 0.5f);
	// const uint32_t g = (uint32_t)((255.0f * srgb.g) + 0.5f);
	// const uint32_t r = (uint32_t)((255.0f * srgb.r) + 0.5f);
	const uint32_t a = (uint32_t)(roundf(255.0f * srgb.a));
	const uint32_t b = (uint32_t)(roundf(255.0f * srgb.b));
	const uint32_t g = (uint32_t)(roundf(255.0f * srgb.g));
	const uint32_t r = (uint32_t)(roundf(255.0f * srgb.r));

	return (a << 24 | b << 16 | g << 8 | r << 0);
}

inline
t_v4 gamma2_unpack(uint32_t packed)
{
    const t_v4 unpacked = rgb_u32_to_v4(packed);
	t_v4 result;

	result.r = gamma2_srgb_to_linear(unpacked.r);
	result.g = gamma2_srgb_to_linear(unpacked.g);
	result.b = gamma2_srgb_to_linear(unpacked.b);
	result.a = unpacked.a;
	return (result);
}
