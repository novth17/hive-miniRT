/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiennguy <hiennguy@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 14:51:03 by hiennguy          #+#    #+#             */
/*   Updated: 2025/07/21 15:09:28 by hiennguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef OBJECT_H
# define OBJECT_H

# include "mini_rt.h"

typedef enum e_obj_type
{
	SPHERE,
	PLANE,
	CYLINDER,
	// later: TRIANGLE, CAT UIIA, etc.
}				t_obj_type;

typedef struct s_object
{
	t_obj_type	type; // type of the shape
	void		*data;  // pointer to t_sphere, t_plane, etc.
}				t_object;

typedef struct s_sphere
{
	t_vec3	center;
	float	diameter;
	t_color	color; //0-255
}				t_sphere;

typedef struct s_plane
{
	t_vec3	point;
	t_vec3	norm_axis;
	t_color	color; //0-255
}				t_plane;

typedef struct s_cylinder
{
	t_vec3	center;
	t_vec3	norm_axis;
	float	diameter;
	float	height;
	t_color	color;//0-255
}				t_cylinder;

#endif
