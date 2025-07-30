#include "mini_rt.h"

t_hit create_plane_hit_record(const t_ray ray, const t_sphere sp, const float root)
{
	t_hit rec;


	return (rec);
}


bool plane_hit(const t_plane pl, const t_ray ray, float *out_t)
{
	float t;
	float denom;
	float D;

	denom = dot(pl.axis, ray.direction);

	if (fabsf(denom) < 1e-6f) // Ray is parallel to plane
		return false;

	D = dot(pl.axis, pl.point);
	t = (D - dot(pl.axis, ray.origin)) / denom;

	if (t < MIN_HIT_DIST || t > MAX_HIT_DIST)
		return false;
	*out_t = t;
	return true;
}

t_hit create_plane_hit_record(const t_ray ray, const t_sphere sp, const float root)
{
	t_hit rec;

	rec.did_hit = true;
	rec.distance = root;
	rec.position = at(ray, rec.distance);
	rec.mat = sp.material;
	rec.normal = v3_div_f32(V3_SUB(rec.position, sp.center), sp.radius);
	rec.front_face = dot(ray.direction, rec.normal) < 0;
	if (rec.front_face == false)
	{
		rec.normal = neg(rec.normal);
	}
	return (rec);
}


static inline
t_hit create_plane_hit_record(const t_ray ray, const t_plane pl, const float t)
{
	t_hit rec;

	rec.distance = t;
	rec.position = vec_add(ray.origin, vec_mul(ray.direction, t));

	// Face orientation
	t_v3 outward_normal = vec_normalize(pl.axis);
	rec.front_face = dot(ray.direction, outward_normal) < 0;
	rec.normal = rec.front_face ? outward_normal : vec_mul(outward_normal, -1.0f);

	rec.color = pl.material.color;  // assuming material contains color
	rec.mat = pl.material;
	rec.did_hit = true;

	return rec;
}
