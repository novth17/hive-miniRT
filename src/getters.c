/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getters.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 19:13:13 by ltaalas           #+#    #+#             */
/*   Updated: 2025/08/28 19:13:13 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "task.h"
#include "camera.h"

t_task	*get_default_task(void)
{
	static t_task	default_task = {};

	return (&default_task);
}

t_camera	*get_frame_cam(void)
{
	static t_camera	frame_cam = {};

	return (&frame_cam);
}
