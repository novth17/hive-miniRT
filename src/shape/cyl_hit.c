/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cyl_hit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiennguy <hiennguy@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 17:17:58 by hiennguy          #+#    #+#             */
/*   Updated: 2025/08/28 17:18:17 by hiennguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

static void		cylinder_set_normal(const t_ray ray, const t_cylinder cyl,
					const t_hit record, t_hit *out);
static t_hit	create_cyl_hit_record(const t_ray ray, const t_cylinder cyl,
					const float t);

float	check_cyl(t_hit *restrict rec, const t_cylinder *cyl,
		const uint32_t count, const t_ray ray)
{
	uint32_t	i;
	float		t;
	float		closest;

	i = 0;
	closest = rec->distance;
	while (i < count)
	{
		t = cyl_hit(cyl[i], ray);
		if (t < closest)
		{
			*rec = create_cyl_hit_record(ray, cyl[i], t);
			closest = t;
		}
		i++;
	}
	return (closest);
}

/**
 * cylinder_set_normal:
 * --------------------
 * Decides which surface of the cylinder the ray hit (top cap, bottom cap,
 * or side),  and sets the correct normal direction at the hit point.
 *
 * Parameters:
 *   ray  - the incoming ray
 *   cyl  - the cylinder that was hit
 *   rec  - the hit record with position already filled
 *   out  - pointer to the hit record to update with the correct normal
 */
static void	cylinder_set_normal(const t_ray ray, const t_cylinder cyl,
		const t_hit record, t_hit *out)
{
	t_cyl_hit	h;

	h.vec_to_hit = v3_sub_v3(record.position, cyl.center);
	h.proj_len = dot(h.vec_to_hit, cyl.axis);
	h.half_height = cyl.height * 0.5f;
	if (h.proj_len < -h.half_height + CYL_CAP_EPSILON)
		out->normal = neg(cyl.axis);
	else if (h.proj_len > h.half_height - CYL_CAP_EPSILON)
		out->normal = cyl.axis;
	else
	{
		h.axis_projection = v3_mul_f32(cyl.axis, h.proj_len);
		h.side_normal = v3_sub_v3(h.vec_to_hit, h.axis_projection);
		out->normal = h.side_normal;
	}
	out->front_face = dot(ray.direction, out->normal) < 0;
	if (!out->front_face)
		out->normal = neg(out->normal);
}

/**
 * Creates a hit record for a cylinder intersection.
 *
 * Determines whether the hit occurred on the side or on one of the caps,
 * computes the correct surface normal, and fills in all fields of the hit
 * record.
 *
 * @param ray   The ray that hit the cylinder.
 * @param cyl   The cylinder that was hit.
 * @param t     Distance along the ray to the hit point.
 *
 * @return A fully populated t_hit struct.
 */
static t_hit	create_cyl_hit_record(const t_ray ray, const t_cylinder cyl,
		const float t)
{
	t_hit	rec;

	rec.distance = t;
	rec.position = at(ray, t);
	rec.mat = cyl.material;
	if (rec.mat.has_checker)
		rec.mat.color = checker_cylinder(rec.position, &cyl);
	rec.did_hit = true;
	cylinder_set_normal(ray, cyl, rec, &rec);
	return (rec);
}
