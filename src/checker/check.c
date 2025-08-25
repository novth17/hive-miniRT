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
    t_v3 local = V3_SUB(p, sphere->center); //maybe no need normalize?

    double phi = atan2(local.z, local.x);
    double theta = asin(local.y);

    // Scale with suze maybe i need a scaler???
    int u = floor((phi + M_PI) / (M_PI / CHECKER_SIZE));
    int v = floor(theta / (M_PI / CHECKER_SIZE));

    if ((u + v) % 2 == 0)
        return (sphere->material.color);
    return v3(0, 0, 0);
}

t_color checker_cylinder(t_v3 p, const t_cylinder *cyl)
{
    double phi = atan2(p.z - cyl->center.z, p.x - cyl->center.x);
    double h   = p.y - cyl->center.y;

	//angular position around the cylinder.
	//h = height position along the y-axis.

    // Scale with suze maybe i need a scaler???
    int u = floor((phi + M_PI) / (M_PI / CHECKER_SIZE));
    int v = floor(h / CHECKER_SIZE);

	// cyl-material.color;
    if ((u + v) % 2 != 0)
	{
		return (cyl->material.color);
	}
	return (v3(0, 0, 0));
}



