#include "mini_rt.h"

t_color checker_plane(t_v3 p, t_material *m, t_object *obj)
{
    int ix = floor(p.x);
    int iz = floor(p.z);

    if ((ix + iz) % 2 == 0)
        return m->color;
    return (t_v3){{0,0,0}};
}

t_color checker_sphere(t_v3 p, t_material *m, t_object *obj)
{
    t_v3 local = normalize(v3_sub(p, obj->sphere.center));

    double phi = atan2(local.z, local.x);
    double theta = acos(local.y);        

    int u = floor((phi + M_PI) / (M_PI/4));
    int v = floor(theta / (M_PI/4));

    if ((u + v) % 2 == 0)
        return m->color;
    return (t_v3){{0,0,0}};
}

t_color checker_cylinder(t_v3 p, t_material *m, t_object *obj)
{
    double phi = atan2(p.z - obj->cyl.center.z, p.x - obj->cyl.center.x);
    double h = p.y - obj->cyl.center.y;

    int u = floor((phi + M_PI) / (M_PI/4));
    int v = floor(h);

    if ((u + v) % 2 == 0)
        return m->color;
    return (t_v3){{0,0,0}};
}


t_color get_surface_color(t_material *m, t_v3 hit_point, t_obj_type type, t_object *obj)
{
    if (!m->has_checker)
        return m->color;

    if (type == PLANE)
        return checker_plane(hit_point, m, obj);
    else if (type == SPHERE)
        return checker_sphere(hit_point, m, obj);
    else if (type == CYLINDER)
        return checker_cylinder(hit_point, m, obj);

    return m->color;
}
