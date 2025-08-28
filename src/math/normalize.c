/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   normalize.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 19:14:03 by ltaalas           #+#    #+#             */
/*   Updated: 2025/08/28 19:14:03 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt_math.h"

inline
t_v3	unit_vector(t_v3 a)
{
	t_v3	result;

	result = v3_div_f32(a, length(a));
	return (result);
}

inline
t_v3	normalize(t_v3 a)
{
	const float	len = length(a);
	t_v3		result;

	result.x = a.x * (1.0f / len);
	result.y = a.y * (1.0f / len);
	result.z = a.z * (1.0f / len);
	return (result);
}

inline
t_v3	noz(t_v3 a)
{
	const float	lensq = length_sq(a);
	float		len;
	t_v3		result;

	result = (t_v3){};
	if (lensq > square(0.0001f))
	{
		len = square_root(lensq);
		result.x = a.x * (1.0f / len);
		result.y = a.y * (1.0f / len);
		result.z = a.z * (1.0f / len);
	}
	return (result);
}
