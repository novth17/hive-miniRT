#ifndef RT_MATH_H
# define RT_MATH_H


# include <float.h>
# include <math.h>
# include "types.h"

// vec2.c
/////////////////////////////
t_v2		v2(float x, float y);
/////////////////////////////

// vec3_1.c
/////////////////////////////
t_v3		v3(float x, float y, float z); // create a vec3
t_v3		neg(t_v3 a); // negate a vec3
t_v3		v3_add_v3(t_v3 a, t_v3 b); // vec3 a + vec3 b
# define V3_ADD v3_add_v3

t_v3		v3_add_f32(t_v3 a, float b); // vec3 a + float b
t_v3		v3_sub_v3(t_v3 a, t_v3 b); // vec3 a - vec3 b
# define V3_SUB v3_sub_v3
/////////////////////////////

// vec3_2.c
/////////////////////////////
t_v3		v3_mul_v3(t_v3 a, t_v3 b); // vec3 a * vec3 b
t_v3		v3_mul_f32(t_v3 a, float b);
t_v3		f32_mul_v3(float a, t_v3 b); // float a * vec3 b;

t_v3		f32_div_v3(float a, t_v3 b);
t_v3		v3_div_f32(t_v3 a, float b);
/////////////////////////////

// normalize.c
/////////////////////////////
t_v3		unit_vector(t_v3 a);
t_v3		normalize(t_v3 a);
t_v3		noz(t_v3 a);
/////////////////////////////

// vec4.c
/////////////////////////////
t_v4		v4(float x, float y, float z, float w);
t_v4		v4_mul_f32(t_v4 a, float b);
t_v4		v4_add(t_v4 a, t_v4 b);
t_v4		v3_to_v4(t_v3 v3, float w);
/////////////////////////////

// quaternion.c
/////////////////////////////
t_v4		quaternion_conjugate(t_v4 q);
t_v4		q_mul_q(t_v4 a, t_v4 b);
t_v4		q_mul_v3(t_v4 q, t_v3 v);
t_v4		quaternion_from_axis_angle(t_v3 axis, float angle);
t_v3		rotate(t_v3 direction, t_v3 axis, float angle);
/////////////////////////////

// interpolate.c
/////////////////////////////
float		lerp(float a, float t, float b);
t_v3		v3_lerp(t_v3 a, float t, t_v3 b);
float		smoothstep(const float edge0, const float edge1, float x);
t_v4		v4_lerp(t_v4 a, float t, t_v4 b);
/////////////////////////////

// square.c
/////////////////////////////
float		square_root(float a);
float		square(float a);
float		length_sq(t_v3 a);
/////////////////////////////

// dot_cross_length.c
/////////////////////////////
float		dot(t_v3 a, t_v3 b);
t_v3		cross(t_v3 a, t_v3 b);
float		length(t_v3 a);
/////////////////////////////

// rt_math.c / misc_math
/////////////////////////////
t_v3		at(const t_ray r, const float t);
float		sign(float value);
t_v3		hadamard(t_v3 a, t_v3 b); // hadamard attennuation
float		clamp(const float f, const float min, const float max);
t_v3		v3_clamp(const t_v3 a);
/////////////////////////////

// color_correction.c
/////////////////////////////
float		exact_linear_to_srgb(float l);
uint32_t	exact_pack(t_v4 unpacked);
float		exact_srgb_to_linear(float srgb);
t_v4		exact_unpack(uint32_t packed);
/////////////////////////////

// random.c
/////////////////////////////
float		random_float(uint32_t *seed);
t_v3		in_unit_sphere(uint32_t *seed);
t_v3		random_in_unit_disk(uint32_t *seed);
t_v3		random_direction(uint32_t *seed);
t_v2		sample_square_stratified(
				int i_s,
				int j_s,
				float recip_sqrt_spp,
				uint32_t *seed);
/////////////////////////////

// random2.c
/////////////////////////////
t_v2		random_point_in_circle(uint32_t *state);
t_v2		sample_square(uint32_t *seed);
float		random_float_normal_dist(uint32_t *seed);
t_v3		random_direction_normal_dist(uint32_t *seed);
t_v3		random_direction_in_hemisphere(
				const t_v3 normal,
				uint32_t *rng_seed);
/////////////////////////////

#endif
