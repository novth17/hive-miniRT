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
t_v2 v2s(int32_t x, int32_t y)
{
	return ((t_v2){.x = (float)x, .y = (float)y});
}

inline
t_v2 v2u(uint32_t x, uint32_t y)
{
	return ((t_v2){.x = (float)x, .y = (float)y});
}

inline
t_v2 v2(float x, float y)
{
	return ((t_v2){.x = x, .y = y});
}

inline
t_v4 v4(float x, float y, float z, float w)
{
	return ((t_v4){.x = x, .y = y, .z = z, .w = w});
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
static inline
float saturate(float f)
{
	if (f < 0.0f)
		return (0.0f);
	else if (f > 1.0f)
		return (1.0f);
	else
		return (f);
	// return ((f < 0.0f) ? 0.0f : (f > 1.0f) ? 1.0f : f);
}

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
	return ((saturate(f)) * 255.0f + 0.5f); // + 0.5f ?
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


inline
float exact_srgb_to_linear(float srgb)
{
    float l;

    if (srgb < 0.0f)
    {
	    srgb = 0.0f;
    }
    if (srgb > 1.0f)
    {
	    srgb = 1.0f;
    }
    l = srgb * 0.0773993808;
    if (srgb > 0.04045)
    {
	    l = pow(srgb * 0.9478672986 + 0.0521327014, 2.4);
    }
    return (l);
}



inline
float exact_linear_to_srgb(float l)
{
    float s;

    if (l < 0.0f)
    {
	    l = 0.0f;
    }
    if (l > 1.0f)
    {
	    l = 1.0f;
    }
    s = l * 12.92;
    if (l > 0.0031308)
    {
	    s = 1.055F*pow(l, 1.0f/2.4f) - 0.055f;
    }
    return (s);
}

uint32_t exact_pack(t_v3 unpacked)
{
    const t_v3 srgb = {
			.r = exact_linear_to_srgb(unpacked.r),
			.g = exact_linear_to_srgb(unpacked.g),
            .b = exact_linear_to_srgb(unpacked.b)};
	uint32_t result;


	result = ((uint32_t)(0xFF) << 24)					|
			((uint32_t)((srgb.b * 255.0f)) << 16)	|
			((uint32_t)((srgb.g * 255.0f)) << 8)	|
			((uint32_t)((srgb.r * 255.0f)) << 0);
	return (result);
}
