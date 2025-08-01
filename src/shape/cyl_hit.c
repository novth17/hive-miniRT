#include "mini_rt.h"

// How far away along the ray we hit the cylinder side.
// If no hit → return -1.

typedef struct s_cyl_math {
	float a;
	float b;
	float c;
	float t0;
	float t1;
	float discriminant;
	float sqrt_d;
}	t_cyl_math;

float cyl_solve(t_ray ray, t_cylinder cyl, t_vec3 oc)
{
	t_cyl_math m;
	float radius = cyl.diameter / 2;
	float d_dot_n = dot(ray.direction, cyl.axis); //dir dot normal
	float oc_dot_n = dot(oc, cyl.axis); // oc dot  normal

	m.a = dot(ray.direction, ray.direction) - d_dot_n * d_dot_n;
	m.b = 2.0f * (dot(ray.direction, oc) - d_dot_n * oc_dot_n);
	m.c = dot(oc, oc) - oc_dot_n * oc_dot_n - radius * radius;

	m.discriminant = m.b*m.b - 4*m.a*m.c;
	if (m.discriminant < 0)
		return (FLT_MAX);

	m.sqrt_d = sqrtf(m.discriminant);
	m.t0 = ;
	m.t1 =



}

float cyl_hit(const t_cylinder cyl, const t_ray ray)
{
	t_vec3 oc;

	oc = v3_sub_v3(ray.origin, cyl.center); // origin to center



	return ();
}

// /*
// - If the dot product is negative, the ray is hitting
// the front face (i.e. opposite to the normal).
// - If it’s positive, the ray is hitting the back side
// (i.e. same direction as the normal).
// If it hits the front: use the normal as-is.
// If it hits the back: flip the normal so it still
// points against the ray direction.
// */
// t_hit	create_plane_hit_record(const t_ray ray, const t_plane pl, const float t)
// {
// 	t_hit	rec;
// 	t_v3	face_normal;

// 	rec.color = pl.material.color;
// 	rec.mat = pl.material;
// 	rec.did_hit = true;
// 	rec.distance = t;
// 	rec.position = at(ray, rec.distance);
// 	face_normal = pl.axis;
// 	rec.front_face = dot(ray.direction, face_normal) < 0;
// 	if (rec.front_face)
// 		rec.normal = face_normal;
// 	else
// 		rec.normal = neg(face_normal);
// 	return (rec);
// }

// float check_cyl(t_hit *restrict rec, const t_plane *planes, const uint32_t count, const t_ray ray)
// {
// 	uint32_t i;
// 	float t;
// 	float closest;

// 	i = 0;
// 	closest = rec->distance;
// 	while (i < count)
// 	{
// 		t = plane_hit(planes[i], ray);
// 		if (t < closest)
// 		{
// 			*rec = create_cyl_hit_record(ray, planes[i], t);
// 			closest = t;
// 		}
// 		i++;
// 	}
// 	return (closest);
// }
