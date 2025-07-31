#include "mini_rt.h"

//define intersection

bool intersect_plane(t_ray ray, t_plane pl, t_hit *rec)
{
	float denom = dot(pl.axis, ray.direction);
	if (fabs(denom) < 1e-6) // Ray is parallel to plane
		return false;

	t_v3 diff = v3_sub_v3(pl.point, ray.origin);
	float t = dot(diff, pl.axis) / denom;
	if (t < 0)
		return false;

	rec->distance = t;
	rec->position = v3_add_v3(ray.origin, v3_mul_v3(ray.direction, t));
	rec->normal = vec_normalize(pl.axis);
	rec->color = pl.material.color;        // assuming material has a color field
	rec->mat = pl.material;
	rec->did_hit = true;

	// optional: set front_face based on ray direction
	rec->front_face = dot(ray.direction, rec->normal) < 0;
	if (!rec->front_face)
		rec->normal = vec_mul(rec->normal, -1.0f); // flip normal if hitting from behind

	return true;
}
