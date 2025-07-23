/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiennguy <hiennguy@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 22:15:36 by hiennguy          #+#    #+#             */
/*   Updated: 2025/07/23 22:31:15 by hiennguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

typedef struct s_scene
{
	t_ambient	ambient;
	t_camera	camera;
	t_light		light;

	t_object	*objects;  // list of objects*
	bool		is_valid;
}	t_scene;

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

typedef struct s_sphere {
	t_vec3	center;
	float	diameter;
	t_color	color;
} t_sphere;



int parse_sphere(char **tokens, t_scene *scene)
{
	t_sphere

	scene->is_valid = true;
	if (check_id_args_count(tokens, "Sphere", 4) == FAIL)
		return (FAIL);


	scene->objects->type = SPHERE;

	return (SUCCESS);
}
