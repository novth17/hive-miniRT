#include "mini_rt.h"

//define intersection
bool intersectPlane

static inline
t_hit create_plane_hit_record(const t_ray ray, const t_sphere sp, const float root)
{
	t_hit rec;


	return (rec);
}

float plane_hit(const t_plane pl, const t_ray ray)
{

}
bool hit_plane(t_ray ray, t_plane plane, double *t_out) {
    double denom = dot(plane.normal, ray.direction);

    if (fabs(denom) < 1e-6) // tia song song mặt phẳng
        return false;

    t_vec3 p0l0 = vec_sub(plane.point, ray.origin);
    double t = dot(p0l0, plane.normal) / denom;

    if (t < 0) // mặt phẳng nằm phía sau camera
        return false;

    *t_out = t;
    return true;
}
