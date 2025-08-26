// t_color checker_sphere(t_v3 normal, t_v3 p, const t_sphere *sphere)
// {
//     // t_v3 local = p;//normalize(V3_SUB(p, sphere->center)); //maybe no need normalize?

//     double phi = atan2(-normal.z, normal.x + M_PI);
//     double theta = acos(-normal.y);

//     // Scale with suze maybe i need a scaler???
//     double u = (phi / (2 * M_PI));
//     double v = (theta / (M_PI));

// 	int x = floor((1.0f / CHECKER_SIZE) * p.x);
// 	int y = floor((1.0f / CHECKER_SIZE) * p.y);
// 	int z = floor((1.0f / CHECKER_SIZE) * p.z);

// 	u = clamp(u, 1, 0);
// 	v = 1.0 - clamp(v, 1, 0);

// 	x = x * (int)u;
// 	y = y * (int)v;
//     if ((x + y) % 2 == 0)
//         return (sphere->material.color);
//     return v3(0, 0, 0);
// }
