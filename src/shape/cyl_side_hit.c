/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cyl_side_hit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiennguy <hiennguy@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 17:15:01 by hiennguy          #+#    #+#             */
/*   Updated: 2025/08/28 17:16:41 by hiennguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

static float	check_in_bound(t_ray ray, t_cylinder cyl, t_cyl_math *m);
static float	solve_cyl(t_ray ray, t_cylinder cyl, t_vec3 oc, t_cyl_math *m);
static float	hit_cyl_cap(t_ray ray, t_vec3 center, t_vec3 normal,
					float radius);

float	cyl_hit(const t_cylinder cyl, const t_ray ray)
{
	t_cyl_math	m;
	t_vec3		oc;
	float		t_side;
	float		t_min;
	t_vec3		half_height;

	half_height = v3_mul_f32(cyl.axis, cyl.height * 0.5f);
	m.cap_bottom = v3_sub_v3(cyl.center, half_height);
	m.cap_top = v3_add_v3(cyl.center, half_height);
	m.radius = cyl.diameter / 2;
	m.t_cap_bottom = hit_cyl_cap(ray, m.cap_bottom, neg(cyl.axis), m.radius);
	m.t_cap_top = hit_cyl_cap(ray, m.cap_top, cyl.axis, m.radius);
	oc = v3_sub_v3(ray.origin, cyl.center);
	t_side = solve_cyl(ray, cyl, oc, &m);
	t_min = t_side;
	if (m.t_cap_bottom < t_min)
		t_min = m.t_cap_bottom;
	if (m.t_cap_top < t_min)
		t_min = m.t_cap_top;
	if (t_min < FLT_MAX)
		return (t_min);
	else
		return (FLT_MAX);
}

/**
 * Hit a ray with a disk cap of a cylinder.
 *
 * @param ray     The ray being tested.
 * @param center  Center of the cap (either base or top).
 * @param normal  Normal of the cap (same as cylinder axis or negated).
 * @param radius  Radius of the cap (cylinder.diameter / 2).
 *
 * @return Distance t along the ray to the intersection with the cap,
 * or FLT_MAX if the ray misses or hits outside the cap.
 */
static float	hit_cyl_cap(t_ray ray, t_vec3 center, t_vec3 normal,
		float radius)
{
	float	denom;
	float	t;
	t_vec3	p;
	t_vec3	vec;

	denom = dot(ray.direction, normal);
	if (fabs(denom) < RAY_PARALLEL_EPSILON)
		return (FLT_MAX);
	t = dot(v3_sub_v3(center, ray.origin), normal) / denom;
	if (t < 0)
		return (FLT_MAX);
	p = at(ray, t);
	vec = v3_sub_v3(p, center);
	if (dot(vec, vec) <= radius * radius)
		return (t);
	return (FLT_MAX);
}

/**
 * Solves the quadratic equation for the intersection of a ray with the side of
 * an infinite cylinder,
 * and filters the result to only return hits within the finite height of the
 * actual cylinder.
 *
 * @param ray  The ray being tested.
 * @param cyl  The finite cylinder to intersect.
 * @param oc   Vector from ray.origin to cyl.center (precomputed to avoid
 * recomputing it).
 *
 * @return Distance t along the ray to the first valid intersection within
 * the cylinder's height,
 * or FLT_MAX if there is no valid hit.
 */
static float	solve_cyl(t_ray ray, t_cylinder cyl, t_vec3 oc, t_cyl_math *m)
{
	m->d_dot_n = dot(ray.direction, cyl.axis);
	m->oc_dot_n = dot(oc, cyl.axis);
	m->a = dot(ray.direction, ray.direction) - m->d_dot_n * m->d_dot_n;
	m->b = 2.0f * (dot(ray.direction, oc) - m->d_dot_n * m->oc_dot_n);
	m->c = dot(oc, oc) - m->oc_dot_n * m->oc_dot_n - m->radius * m->radius;
	m->discriminant = m->b * m->b - 4 * m->a * m->c;
	if (m->discriminant < 0)
		return (FLT_MAX);
	if (fabs(m->a) < QUADRATIC_A_EPSILON)
		return (FLT_MAX);
	m->sqrt_d = square_root(m->discriminant);
	m->t0 = (-m->b - m->sqrt_d) / (2.0f * m->a);
	m->t1 = (-m->b + m->sqrt_d) / (2.0f * m->a);
	return (check_in_bound(ray, cyl, m));
}

/**
* Checks whether the intersection points (t0 and t1) lie within the finite
* height of the cylinder.
*
* @param ray  The ray being tested.
* @param cyl  The finite cylinder to test intersection with.
* @param m    Struct containing precomputed math values, including t0 and t1.
*
* @return The nearest valid t (distance along the ray) that hits within the
* cylinder bounds,or FLT_MAX if no such hit is found.
*/
static float	check_in_bound(t_ray ray, t_cylinder cyl, t_cyl_math *m)
{
	float	t_valid;

	t_valid = FLT_MAX;
	if (m->t0 >= 0)
	{
		m->hit0 = at(ray, m->t0);
		m->vec_to_hit0 = v3_sub_v3(m->hit0, m->cap_bottom);
		m->hit_dist_from_base0 = dot(m->vec_to_hit0, cyl.axis);
		if (m->hit_dist_from_base0 >= 0 && m->hit_dist_from_base0 <= cyl.height)
			t_valid = m->t0;
	}
	if (m->t1 >= 0)
	{
		m->hit1 = at(ray, m->t1);
		m->vec_to_hit1 = v3_sub_v3(m->hit1, m->cap_bottom);
		m->hit_dist_from_base1 = dot(m->vec_to_hit1, cyl.axis);
		if (m->hit_dist_from_base1 >= 0 && m->hit_dist_from_base1 <= cyl.height)
		{
			if (m->t1 < t_valid)
				t_valid = m->t1;
		}
	}
	return (t_valid);
}
