#include "mini_rt.h"

bool plane_hit(const t_plane pl, const t_ray ray, float *t)
{
	float denom = dot(pl.axis, ray.direction);

	if (fabsf(denom) < 1e-6f) // Ray is parallel to plane
		return false;

	*t = dot(pl.axis, v3_sub_v3(pl.point, ray.origin)) / denom;

	if (*t < MIN_HIT_DIST || *t > MAX_HIT_DIST)
		return false;

	return true;
}


/*
- If the dot product is negative, the ray is hitting
the front face (i.e. opposite to the normal).
- If it’s positive, the ray is hitting the back side
(i.e. same direction as the normal).
If it hits the front: use the normal as-is.
If it hits the back: flip the normal so it still
points against the ray direction.
*/
t_hit	create_plane_hit_record(const t_ray ray, const t_plane pl, const float t)
{
	t_hit	rec;
	t_v3	face_normal;

	rec.color = pl.material.color;
	rec.mat = pl.material;
	rec.did_hit = true;
	rec.distance = t;
	rec.position = at(ray, rec.distance);

	face_normal = normalize(pl.axis);//maybe no need normalize?
	rec.front_face = dot(ray.direction, face_normal) < 0;
	if (rec.front_face)
		rec.normal = face_normal;
	else
		rec.normal = neg(face_normal);
	return (rec);
}

float check_planes(t_hit *restrict rec, const t_plane *planes, const uint32_t count, const t_ray ray)
{
	uint32_t i;
	float t;
	float closest;

	closest = rec->distance;
	i = 0;
	while (i < count)
	{
		if (plane_hit(planes[i], ray, &t) && t < closest)
		{
			*rec = create_plane_hit_record(ray, planes[i], t);
			closest = t;
		}
		++i;
	}
	return (closest);
}
