/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 16:38:13 by ltaalas           #+#    #+#             */
/*   Updated: 2025/05/24 22:53:43 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt_math.h"

inline
t_v3	v3(float x, float y, float z)
{
	return ((t_v3)
		{
			.x = x,
			.y = y,
			.z = z,
		});
}

inline
t_v3	v3_add_v3(t_v3 a, t_v3 b)
{
	return ((t_v3)
		{
			.x = a.x + b.x,
			.y = a.y + b.y,
			.z = a.z + b.z,
		});
}

inline
t_v3	v3_add_f32(t_v3 a, float b)
{
	return ((t_v3)
		{
			.x = a.x + b,
			.y = a.y + b,
			.z = a.z + b,
		});
}

inline
t_v3	v3_sub_v3(t_v3 a, t_v3 b)
{
	return ((t_v3)
		{
			.x = a.x - b.x,
			.y = a.y - b.y,
			.z = a.z - b.z,
		});
}

inline
t_v3	v3_mul_v3(t_v3 a, t_v3 b)
{
	return ((t_v3)
		{
			.x = a.x * b.x,
			.y = a.y * b.y,
			.z = a.z * b.z,
		});
}

inline
t_v3	f32_mul_v3(float a, t_v3 b)
{
	return ((t_v3)
		{
			.x = a * b.x,
			.y = a * b.y,
			.z = a * b.z,
		});
}

inline
t_v2	v2(float x, float y)
{
	return ((t_v2)
		{
			.x = x,
			.y = y,
		});
}

inline
t_v2	v2u(uint32_t x, uint32_t y)
{
	return ((t_v2)
		{
			.x = (float)x,
			.y = (float)y,
		});
}

inline
t_v2	v2s(int32_t x, int32_t y)
{
	return ((t_v2)
		{
			.x = (float)x,
			.y = (float)y,
		});
}

inline
t_v4	v4(float x, float y, float z, float w)
{
	return ((t_v4)
		{
			.x = x,
			.y = y,
			.z = z,
			.w = w,
		});
}
