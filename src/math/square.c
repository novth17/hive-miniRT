/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   square.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 19:14:14 by ltaalas           #+#    #+#             */
/*   Updated: 2025/08/28 19:14:19 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt_math.h"

inline
float	square(float a)
{
	float	result;

	result = a * a;
	return (result);
}

inline
float	length_sq(t_v3 a)
{
	float	result;

	result = dot(a, a);
	return (result);
}

inline
float	square_root(float a)
{
	float	result;

	result = sqrtf(a);
	return (result);
}
