/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiennguy <hiennguy@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 14:51:03 by hiennguy          #+#    #+#             */
/*   Updated: 2025/07/26 15:55:57 by hiennguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef OBJECT_H
# define OBJECT_H

#include "types.h"

//# include "rt_math_hien.h"
typedef enum e_obj_type
{
	SPHERE,
	PLANE,
	CYLINDER,
	// later: TRIANGLE, CAT UIIA, etc.
}				t_obj_type;

typedef struct
{
	t_color color;
	float diffuse;
	float specular;
	float emitter;
} t_material;

typedef struct s_sphere
{
	t_vec3	center;
	float	diameter;
	t_color	color; //0-255
}				t_sphere;

typedef struct s_plane
{
	t_vec3	point;
	t_vec3	axis;
	t_color	color; //0-255
}				t_plane;

typedef struct s_cylinder
{
	t_vec3	center;
	t_vec3	axis;
	float	diameter;
	float	height;
	t_color	color;//0-255
}				t_cylinder;

typedef struct s_object
{
	t_obj_type	type; // type of the shape
	union {

		t_sphere	sphere;
		t_cylinder	cyl;
		t_plane		pl;
	};
	struct s_object	*next;
}				t_object;




#endif
