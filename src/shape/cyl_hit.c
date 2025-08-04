#include "mini_rt.h"

/**
 * Creates a hit record for a cylinder intersection.
 *
 * Determines whether the hit occurred on the side or on one of the caps,
 * computes the correct surface normal, and fills in all fields of the hit record.
 *
 * @param ray   The ray that hit the cylinder.
 * @param cyl   The cylinder that was hit.
 * @param t     Distance along the ray to the hit point.
 *
 * @return A fully populated t_hit struct.
 */
static t_hit create_cyl_hit_record(const t_ray ray, const t_cylinder cyl, const float t)
{
	t_hit	rec;
	t_v3		hit_point;
	t_v3		vec_to_hit;
	t_v3		axis_projection;
	t_v3		side_normal;
	float		proj_len;

	hit_point = at(ray, t);
	vec_to_hit = v3_sub_v3(hit_point, cyl.center);
	proj_len = dot(vec_to_hit, cyl.axis);

	rec.position = hit_point;
	rec.distance = t;
	//rec.color = cyl.material.color;
	rec.mat = cyl.material;
	rec.did_hit = true;

	float half_height = cyl.height * 0.5f;
	if (proj_len < -half_height + 1e-4f)
		rec.normal = neg(cyl.axis); // bottom cap
	else if (proj_len > half_height - 1e-4f)
		rec.normal = cyl.axis; // top cap
	else
	{
		axis_projection = v3_mul_f32(cyl.axis, proj_len);
		side_normal = v3_sub_v3(vec_to_hit, axis_projection);
		rec.normal = side_normal; // perpendicular to axis
	}

	// Flip normal if ray is inside the surface
	rec.front_face = dot(ray.direction, rec.normal) < 0;
	if (!rec.front_face)
		rec.normal = neg(rec.normal);

	return rec;
}

float check_cyl(t_hit *restrict rec, const t_cylinder *cyl, const uint32_t count, const t_ray ray)
{
	uint32_t i;
	float t;
	float closest;

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
