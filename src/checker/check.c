#include "mini_rt.h"

t_color checker_plane(t_v3 p, t_material *m)
{
    int ix = floor(p.x / CHECKER_SIZE);
    int iz = floor(p.z / CHECKER_SIZE);

    if ((ix + iz) % 2 == 0)
        return m->color;
    return v3(0, 0, 0);
}

t_color checker_sphere(t_v3 p, const t_sphere *sphere)
{
    t_v3 local = normalize(V3_SUB(p, sphere->center)); //maybe no need normalize?

    double phi = atan2(local.z, local.x);
    double theta = acos(local.y);

    int u = (phi + M_PI) * (sphere->radius * CHECKER_DENSITY) / (2.0 * M_PI);
    int v = theta * (sphere->radius  * CHECKER_DENSITY) / M_PI;

    if ((u + v) % 2 == 0)
        return (sphere->material.color);
    return v3(0, 0, 0);
}

t_color checker_cylinder(t_v3 p, const t_cylinder *cyl)
{
    t_v3 local = (V3_SUB(p, cyl->center)); //maybe no need normalize?


    double phi = atan2(local.z, local.x);
    double h   = p.y - cyl->center.y;

	//angular position around the cylinder.
	//h = height position along the y-axis.

    // Scale with suze maybe i need a scaler???
    int u = ((phi + M_PI) * cyl->diameter * CHECKER_DENSITY) / (2.0 * M_PI);
    int v = h * CHECKER_DENSITY / cyl->height;

    if ((u + v) % 2 != 0)
	{
		return (cyl->material.color);
	}
	return (v3(0, 0, 0));
}



