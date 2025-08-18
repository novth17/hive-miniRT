#ifndef HIT_H
# define HIT_H

typedef struct s_cyl_hit
{
	t_v3	hit_point;
	t_v3	vec_to_hit;
	t_v3	axis_projection;
	t_v3	side_normal;
	float	proj_len;
	float	half_height;
}			t_cyl_hit;

/**
 * Struct used to store intermediate values when solving for a ray-cylinder
 * intersection.
 *
 * Members:
 * - a, b, c: coefficients of the quadratic equation for the cylinder's side
 * intersection.
 * - radius: half of the cylinder's diameter.
 * - d_dot_n: dot(ray.direction, cyl.axis), reused multiple times.
 * - oc_dot_n: dot(ray.origin - cyl.center, cyl.axis)
 * - t0, t1: roots of the quadratic equation (possible hit distances).
 * - discriminant: value under the square root in the quadratic formula.
 * - sqrt_d: square root of discriminant.
 * - hit0, hit1: hit points corresponding to t0 and t1.
 * - vec_to_hit0, vec_to_hit1: vectors from cylinder base to hit0 and hit1.
 * - hit_dist_from_base0, hit_dist_from_base1: projected distances of hit points
 * along cylinder axis.
 */
typedef struct s_cyl_math
{
	float	a;
	float	b;
	float	c;
	float	radius;
	float	d_dot_n;
	float	oc_dot_n;
	float	t0;
	float	t1;
	float	discriminant;
	float	sqrt_d;
	t_vec3	vec_to_hit0;
	t_vec3	vec_to_hit1;
	t_vec3	hit0;
	t_vec3	hit1;
	float	hit_dist_from_base0;
	float	hit_dist_from_base1;
	t_vec3	cap_bottom;
	t_vec3	cap_top;
	float	t_cap_bottom;
	float	t_cap_top;
}			t_cyl_math;

typedef struct s_hit_record
{
	t_v3 position; // p
	t_v3	normal;
	float distance;  // t
	bool front_face; // maybe not needed;
	bool did_hit;    // can be removed later
	t_material mat;  // for now maybe have material index or smth idunno
}			t_hit;

#endif
