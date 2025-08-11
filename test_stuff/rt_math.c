#include "rt_math.h"

// @TODO delete probably
// inline
// float Pow(float a, float b)
// {
// 	float result = (float)pow(a, b);
// 	return (result);
// }

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

	result = inner(a, a);
	return (result);
}

#include <math.h>
// @TODO remake mby
inline
float square_root(float a)
{
	float result = (float)sqrt(a);

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
t_v3 v3(float x, float y, float z)
{
	return ((t_v3){.x = x, .y = y, .z = z});
}

inline
t_v4 v4(float x, float y, float z, float w)
{
	return ((t_v4){.x = x, .y = y, .z = z, .w = w});
}

inline
t_v3 v3_add_v3(t_v3 a, t_v3 b)
{
	t_v3 result;

	result.x = a.x + b.x;
	result.y = a.y + b.y;
	result.z = a.z + b.z;
	return (result);
}

inline
t_v3 v3_add_f32(t_v3 a, float b)
{
	t_v3 result;

	result.x = a.x + b;
	result.y = a.y + b;
	result.z = a.z + b;
	return (result);
}

inline
t_v3 v3_sub_v3(t_v3 a, t_v3 b)
{
	t_v3 result;

	result.x = a.x - b.x;
	result.y = a.y - b.y;
	result.z = a.z - b.z;
	return (result);
}

inline
t_v3 v3_mul_v3(t_v3 a, t_v3 b)
{
	t_v3 result;

	result.x = a.x * b.x;
	result.y = a.y * b.y;
	result.z = a.z * b.z;
	return (result);
}

inline
t_v3 f32_mul_v3(float a, t_v3 b)
{
	t_v3 result;

	result.x = a * b.x;
	result.y = a * b.y;
	result.z = a * b.z;
	return (result);
}

inline
t_v3 f32_div_v3(float a, t_v3 b)
{
	t_v3 result;

	result.x = a / b.x;
	result.y = a / b.y;
	result.z = a / b.z;
	return (result);
}

inline
t_v3 v3_div_f32(t_v3 a, float b)
{
	t_v3 result;

	result.x = a.x / b;
	result.y = a.y / b;
	result.z = a.z / b;
	return (result);
}

inline
t_v3 neg(t_v3 a)
{
	t_v3 result;

	result.x = -a.x;
	result.y = -a.y;
	result.z = -a.z;
	return (result);
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

static inline
uint8_t float_to_u8_sat(float f)
{
	return ((saturate(f)) * 255.0f + 0.5f); // + 0.5f ?
}

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

	result = (t_v4){(float)((packed >> 0) & 0xFF),
					(float)((packed >> 8) & 0xFF),
					(float)((packed >> 16) & 0xFF),
					(float)((packed >> 24) & 0xFF)};
	return (result);
}


inline
t_v4 v4_lerp(t_v4 a, float t, t_v4 b)
{
	t_v4 result;

	result =(t_v4)
	{
		.x = (1.0f - t)*a.x + t*b.x,
		.y = (1.0f - t)*a.y + t*b.y,
		.z = (1.0f - t)*a.z + t*b.z,
		.w = (1.0f - t)*a.w + t*b.w,
	};
	return (result);
}

inline
t_v3 v3_lerp(t_v3 a, float t, t_v3 b)
{
	t_v3 result;

	result = (t_v3)
	{
		.x = (1.0f - t)*a.x + t*b.x,
		.y = (1.0f - t)*a.y + t*b.y,
		.z = (1.0f - t)*a.z + t*b.z,
	};
	return (result);
}

inline
float lerp(float a, float t, float b)
{
	float result;

	result = (1.0f - t)*a + t*b;
	return (result);
}


inline
float inner(t_v3 a, t_v3 b)
{
	float result;

	result = a.x*b.x + a.y*b.y + a.z*b.z;
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
t_v3 unit_vector(t_v3 a)
{
	t_v3 result;

	result = v3_div_f32(a, length(a));
	return result;
}

inline
t_v3 normalize(t_v3 a)
{
	const float len = length(a);

	t_v3 result = {
		.x = a.x * (1.0f / len),
		.y = a.y * (1.0f / len),
		.z = a.z * (1.0f / len),

	};
	return (result);
}

inline
t_v3 noz(t_v3 a)
{
	t_v3 result;
	const float lensq = length_sq(a);

	if (lensq > square(0.0001f))
	{
		result.x = a.x * (1.0f / square_root(lensq));
		result.y = a.y * (1.0f / square_root(lensq));
		result.z = a.z * (1.0f / square_root(lensq));
	}
	return (result);
}


inline
t_v3 hadamard(t_v3 a, t_v3 b)
{
	t_v3 result;

	result = (t_v3){a.x*b.x, a.y*b.y, a.z*b.z};
	return (result);
}
