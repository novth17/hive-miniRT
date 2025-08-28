/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere_hit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 19:12:28 by ltaalas           #+#    #+#             */
/*   Updated: 2025/08/28 19:12:30 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

#define A x
#define H y
#define C z

static inline
void	set_face_normal(t_hit *restrict rec, const t_ray *restrict ray)
{
	rec->front_face = dot(ray->direction, rec->normal) < 0;
	if (false == rec->front_face)
		rec->normal = neg(rec->normal);
}

static inline
void	create_sphere_hit_record(t_hit *restrict rec, const t_ray ray,
	const t_sphere sp, const float root)
{
	rec->did_hit = true;
	rec->distance = root;
	rec->position = at(ray, rec->distance);
	rec->mat = sp.material;
	if (rec->mat.has_checker)
		rec->mat.color = checker_sphere(rec->position, &sp);
	rec->normal = noz(v3_div_f32(V3_SUB(rec->position, sp.center), sp.radius));
	set_face_normal(rec, &ray);
}

inline
float	sphere_hit(const t_sphere sp, const t_ray ray)
{
	const t_v3	oc = v3_sub_v3(sp.center, ray.origin);
	const t_v3	v = {
		.A = dot(ray.direction, ray.direction),
		.H = dot(ray.direction, oc),
		.C = dot(oc, oc) - sp.radius * sp.radius};
	const float	discriminant = v.H * v.H - v.A * v.C;
	float		sqrtd;
	float		root;

	if (discriminant < 0)
		return (-1.0f);
	sqrtd = square_root(discriminant);
	root = (v.H - sqrtd) / v.A;
	if (root <= MIN_HIT_DIST || root >= MAX_HIT_DIST)
	{
		root = (v.H + sqrtd) / v.A;
		if (root <= MIN_HIT_DIST || root >= MAX_HIT_DIST)
			return (-1.0f);
	}
	return (root);
}

inline
float	check_spheres(t_hit *restrict rec, const t_sphere *spheres,
	const uint32_t count, const t_ray ray)
{
	uint32_t	i;
	t_sphere	sphere;
	float		hit_distance;
	float		current_hit_distance;

	current_hit_distance = rec->distance;
	i = 0;
	while (i < count)
	{
		sphere = spheres[i];
		hit_distance = sphere_hit(sphere, ray);
		if (hit_distance > MIN_HIT_DIST && hit_distance < current_hit_distance)
		{
			create_sphere_hit_record(rec, ray, sphere, hit_distance);
			current_hit_distance = hit_distance;
		}
		++i;
	}
	return (current_hit_distance);
}
