/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quaternion.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 19:14:05 by ltaalas           #+#    #+#             */
/*   Updated: 2025/08/28 19:14:05 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt_math.h"

inline
t_v4	q_mul_v3(t_v4 q, t_v3 v)
{
	const float	w = -(q.x * v.x) - (q.y * v.y) - (q.z * v.z);
	const float	x = (q.w * v.x) + (q.y * v.z) - (q.z * v.y);
	const float	y = (q.w * v.y) + (q.z * v.x) - (q.x * v.z);
	const float	z = (q.w * v.z) + (q.x * v.y) - (q.y * v.x);

	return (v4(x, y, z, w));
}

inline
t_v4	q_mul_q(t_v4 a, t_v4 b)
{
	const float	w = (a.w * b.w) - (a.x * b.x) - (a.y * b.y) - (a.z * b.z);
	const float	x = (a.x * b.w) + (a.w * b.x) + (a.y * b.z) - (a.z * b.y);
	const float	y = (a.y * b.w) + (a.w * b.y) + (a.z * b.x) - (a.x * b.z);
	const float	z = (a.z * b.w) + (a.w * b.z) + (a.x * b.y) - (a.y * b.x);

	return (v4(x, y, z, w));
}

inline
t_v4	quaternion_conjugate(t_v4 q)
{
	t_v4	result;

	result.w = q.w;
	result.x = -q.x;
	result.y = -q.y;
	result.z = -q.z;
	return (result);
}

inline
t_v4	quaternion_from_axis_angle(t_v3 axis, float angle)
{
	const float	c = sin(angle / 2.0f);
	t_v4		result;

	result.w = cos(angle / 2.0f);
	result.x = c * axis.x;
	result.y = c * axis.y;
	result.z = c * axis.z;
	return (result);
}

// rotate direction around axis by angle
t_v3	rotate(t_v3 direction, t_v3 axis, float angle)
{
	const t_v4	rotation_q = quaternion_from_axis_angle(axis, angle);
	const t_v4	conjugate_q = quaternion_conjugate(rotation_q);
	t_v4		w;
	t_v3		result;

	w = q_mul_v3(rotation_q, direction);
	w = q_mul_q(w, conjugate_q);
	result.x = w.x;
	result.y = w.y;
	result.z = w.z;
	return (result);
}
