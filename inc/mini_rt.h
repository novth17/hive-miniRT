/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_rt.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiennguy <hiennguy@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 20:30:17 by hiennguy          #+#    #+#             */
/*   Updated: 2025/07/22 15:00:31 by hiennguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_RT_H
#define MINI_RT_H

#define SUCCESS	0
#define FAIL	1

# include <fcntl.h>
# include "MLX42.h"
# include "libft.h"
# include "mini_rt.h"

# include "../test_stuff/rt_math.h"

# define WINDOW_WIDTH 800
# define WINDOW_HEIGHT 800

typedef struct s_vec3
{
	float x;
	float y;
	float z;
}				t_vec3;

typedef struct s_color
{
	int r;
	int g;
	int b;
}				t_color;

typedef struct	s_ambient
{
	float	ratio;
	t_color	color;
}			t_ambient;

typedef struct	s_camera
{
	t_vec3	origin;
	t_vec3	direction;  //must be normalized (range [-1,1])
	float	fov; //Horizontal field of view in degrees in range [0,180]:
}	t_camera;

typedef struct	s_light
{
	t_vec3	origin;
	float	bright_ratio; // Brightness ratio in range [0.0, 1.0]
	t_color	color;
}	t_light;

typedef struct s_scene
{
	t_ambient	ambient;
	t_camera	camera;
	t_light		light;

	t_list		*objects;  // list of objects*
}	t_scene;

typedef struct s_mini_rt
{
	mlx_t			*mlx;
	mlx_image_t		*image;
	t_scene			scene;

}	t_minirt;


/* ===================== INPUT PARSING & INIT ===================== */
int		validate_input(int argc, char *filename);
int		init_minirt(t_minirt *minirt, char **argv);
int		parse(t_minirt *minirt, char *filename);

/* ===================== FOR DRAW ===================== */

/* ===================== FOR ERROR ===================== */
void	exit_error(const char *msg);
int		print_ret_error(const char *msg);

# endif
