/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiennguy <hiennguy@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 17:23:03 by hiennguy          #+#    #+#             */
/*   Updated: 2025/08/28 17:23:05 by hiennguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

t_color	checker_plane(t_v3 p, const t_plane *pl)
{
	int	ix;
	int	iy;

	if (fabs(pl->axis.y) > 0.9)
	{
		ix = floor(p.x / CHECKER_SIZE);
		iy = floor(p.z / CHECKER_SIZE);
	}
	else if (fabs(pl->axis.x) > 0.9)
	{
		ix = floor(p.y / CHECKER_SIZE);
		iy = floor(p.z / CHECKER_SIZE);
	}
	else
	{
		ix = floor(p.x / CHECKER_SIZE);
		iy = floor(p.y / CHECKER_SIZE);
	}
	if ((ix + iy) % 2 == 0)
		return (pl->material.color);
	return (v3(0, 0, 0));
}

t_color	checker_sphere(t_v3 p, const t_sphere *sphere)
{
	t_v3	local;
	double	phi;
	double	theta;
	int		u;
	int		v;

	local = normalize(V3_SUB(p, sphere->center));
	phi = atan2(local.z, local.x);
	theta = acos(local.y);
	u = (phi + M_PI) * (sphere->radius * CHECKER_DENSITY) / (2.0 * M_PI);
	v = theta * (sphere->radius * CHECKER_DENSITY) / M_PI;
	if ((u + v) % 2 == 0)
		return (sphere->material.color);
	return (v3(0, 0, 0));
}

t_color	checker_cylinder(t_v3 p, const t_cylinder *cyl)
{
	double	circumference;
	double	phi;
	double	h;
	int		u;
	int		v;

	circumference = M_PI * cyl->diameter;
	phi = atan2(p.z - cyl->center.z, p.x - cyl->center.x);
	h = p.y - cyl->center.y;
	u = (phi + M_PI) * (circumference * CHECKER_DENSITY) / (2.0 * M_PI);
	v = (h + cyl->height * 0.5) * CHECKER_DENSITY;
	if ((u + v) % 2 == 0)
		return (cyl->material.color);
	return (v3(0, 0, 0));
}
