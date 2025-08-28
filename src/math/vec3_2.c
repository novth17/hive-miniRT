/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3_2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 19:14:26 by ltaalas           #+#    #+#             */
/*   Updated: 2025/08/28 19:14:27 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt_math.h"

inline
t_v3	v3_mul_v3(t_v3 a, t_v3 b)
{
	t_v3	result;

	result.x = a.x * b.x;
	result.y = a.y * b.y;
	result.z = a.z * b.z;
	return (result);
}

inline
t_v3	f32_mul_v3(float a, t_v3 b)
{
	t_v3	result;

	result.x = a * b.x;
	result.y = a * b.y;
	result.z = a * b.z;
	return (result);
}

inline
t_v3	v3_mul_f32(t_v3 a, float b)
{
	t_v3	result;

	result.x = a.x * b;
	result.y = a.y * b;
	result.z = a.z * b;
	return (result);
}

inline
t_v3	f32_div_v3(float a, t_v3 b)
{
	t_v3	result;

	result.x = a / b.x;
	result.y = a / b.y;
	result.z = a / b.z;
	return (result);
}

inline
t_v3	v3_div_f32(t_v3 a, float b)
{
	t_v3	result;

	result.x = a.x / b;
	result.y = a.y / b;
	result.z = a.z / b;
	return (result);
}
