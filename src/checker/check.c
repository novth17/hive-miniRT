#include "mini_rt.h"

t_color checker_plane(t_v3 p, t_object *obj)
{
    int ix = floor(p.x / CHECKER_SIZE);
    int iz = floor(p.z / CHECKER_SIZE);

    if ((ix + iz) % 2 == 0)
        return obj->pl.material.color;
    return v3(0, 0, 0); // black
}

t_color checker_sphere(t_v3 p, t_object *obj)
{
    t_v3 local = normalize(V3_SUB(p, obj->sphere.center));
    double phi = atan2(local.z, local.x);   // [-π, π]
    double theta = acos(local.y);           // [0, π]

    // Scale with CHECKER_SIZE
    int u = floor((phi + M_PI) / (M_PI / CHECKER_SIZE));
    int v = floor(theta / (M_PI / CHECKER_SIZE));

    if ((u + v) % 2 == 0)
        return obj->sphere.material.color;
    return v3(0, 0, 0);
}

t_color checker_cylinder(t_v3 p, t_object *obj)
{
    double phi = atan2(p.z - obj->cyl.center.z, p.x - obj->cyl.center.x);
    double h   = p.y - obj->cyl.center.y;

    // Scale with CHECKER_SIZE
    int u = floor((phi + M_PI) / (M_PI / CHECKER_SIZE));
    int v = floor(h / CHECKER_SIZE);

    if ((u + v) % 2 == 0)
        return obj->cyl.material.color;
    return v3(0, 0, 0);
}

t_color get_surface_color(t_material *m, t_v3 hit_point, t_object *obj)
{
    if (!m->has_checker)
        return m->color;

    if (obj->type == PLANE)
        return checker_plane(hit_point, obj);
    else if (obj->type == SPHERE)
        return checker_sphere(hit_point, obj);
    else if (obj->type == CYLINDER)
        return checker_cylinder(hit_point, obj);

    return m->color;
}


