#include "mini_rt.h"

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
static
void	create_cyl_hit_record(t_hit *rec, const t_ray ray, const t_cylinder cyl,
		const float t)
{
	t_cyl_hit	h;

	rec->distance = t;
	rec->position = at(ray, t);
	rec->mat = cyl.material;
	if (rec->mat.has_checker)
		rec->mat.color = checker_cylinder(rec->position, &cyl);
	rec->did_hit = true;
	h.vec_to_hit = v3_sub_v3(rec->position, cyl.center);
	h.proj_len = dot(h.vec_to_hit, cyl.axis);
	h.half_height = cyl.height * 0.5f;
	if (h.proj_len < -h.half_height + CYL_CAP_EPSILON)
		rec->normal = neg(cyl.axis);
	else if (h.proj_len > h.half_height - CYL_CAP_EPSILON)
		rec->normal = cyl.axis;
	else
	{
		h.axis_projection = v3_mul_f32(cyl.axis, h.proj_len);
		h.side_normal = v3_sub_v3(h.vec_to_hit, h.axis_projection);
		rec->normal = h.side_normal;
	}
	rec->front_face = dot(ray.direction, rec->normal) < 0;
	if (!rec->front_face)
		rec->normal = neg(rec->normal);
}

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
			create_cyl_hit_record(rec, ray, cyl[i], t);
			closest = t;
		}
		i++;
	}
	return (closest);
}
