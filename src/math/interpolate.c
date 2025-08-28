/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpolate.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 19:14:01 by ltaalas           #+#    #+#             */
/*   Updated: 2025/08/28 19:14:01 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt_math.h"

inline
float	lerp(float a, float t, float b)
{
	float	result;

	result = (1.0f - t) * a + t * b;
	return (result);
}

inline
t_v3	v3_lerp(t_v3 a, float t, t_v3 b)
{
	t_v3	result;

	result.x = (1.0f - t) * a.x + t * b.x;
	result.y = (1.0f - t) * a.y + t * b.y;
	result.z = (1.0f - t) * a.z + t * b.z;
	return (result);
}

inline
float	smoothstep(const float edge0, const float edge1, float x)
{
	x = clamp((x - edge0) / (edge1 - edge0), edge0, edge1);
	return (x * x * (3.0f - 2.0f * x));
}

inline
t_v4	v4_lerp(t_v4 a, float t, t_v4 b)
{
	t_v4	result;

	result.x = (1.0f - t) * a.x + t * b.x;
	result.y = (1.0f - t) * a.y + t * b.y;
	result.z = (1.0f - t) * a.z + t * b.z;
	result.w = (1.0f - t) * a.w + t * b.w;
	return (result);
}
