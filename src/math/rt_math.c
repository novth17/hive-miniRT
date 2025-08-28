/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_math.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 19:14:17 by ltaalas           #+#    #+#             */
/*   Updated: 2025/08/28 19:14:17 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt_math.h"

inline // @TODO move this
t_v3	at(const t_ray r, const float t)
{
	t_v3	result;

	result = V3_ADD(r.origin, f32_mul_v3(t, r.direction));
	return (result);
}

inline
float	sign(float value)
{
	if (value < 0.0)
		return (-1.0f);
	else if (value > 0.0)
		return (1.0f);
	else
		return (0.0f);
}

inline
t_v3	hadamard(t_v3 a, t_v3 b)
{
	t_v3	result;

	result = v3(a.x * b.x, a.y * b.y, a.z * b.z);
	return (result);
}

inline
float	clamp(const float f, const float min, const float max)
{
	if (f < min)
		return (min);
	else if (f > max)
		return (max);
	else
		return (f);
}

inline
t_v3	v3_clamp(const t_v3 a)
{
	t_v3	result;

	result.x = clamp(a.x, 0, 1);
	result.y = clamp(a.y, 0, 1);
	result.z = clamp(a.z, 0, 1);
	return (result);
}
