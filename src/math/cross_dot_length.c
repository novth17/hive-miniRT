/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cross_dot_length.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 19:13:58 by ltaalas           #+#    #+#             */
/*   Updated: 2025/08/28 19:13:59 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt_math.h"

inline
t_v3	cross(t_v3 a, t_v3 b)
{
	t_v3	result;

	result.x = (a.y * b.z) - (a.z * b.y);
	result.y = (a.z * b.x) - (a.x * b.z);
	result.z = (a.x * b.y) - (a.y * b.x);
	return (result);
}

inline
float	dot(t_v3 a, t_v3 b)
{
	float	result;

	result = (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
	return (result);
}

inline
float	length(t_v3 a)
{
	float	result;

	result = square_root(length_sq(a));
	return (result);
}
