/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_rt.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiennguy <hiennguy@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 20:30:17 by hiennguy          #+#    #+#             */
/*   Updated: 2025/07/26 17:26:45 by hiennguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_RT_H
#define MINI_RT_H

#define SUCCESS	0
#define FAIL	1






# include <fcntl.h>
// # include "MLX42.h"
// # include "libft.h"
// # include "mini_rt.h"


# include "../libs/libft/inc/libft.h"
# include "rt_math.h"
# include "../libs/MLX42/include/MLX42/MLX42.h"

typedef struct
{
	float	aspect_ratio; //
	int32_t image_width;
	int32_t image_height;
	int32_t samples_per_pixel;
	float	pixel_sample_scale;
	int32_t max_bounce; // for mandatory 1 or 2 i guess;

	float		fov;
	t_point3	lookfrom; // @QUESTION why this and camera_center??
	t_point3	lookat;
	t_v3		vup;

 	float defocus_angle;  // Variation angle of rays through each pixel
    float focus_dist;

	t_v3 defocus_disk_u;
	t_v3 defocus_disk_v;



	// float focal_length;
	float viewport_height;
	float viewport_width;
	t_v3 camera_center;

	t_v3 viewport_u;
	t_v3 viewport_v;

	t_v3 pixel_delta_u;
	t_v3 pixel_delta_v;

	t_v3 viewport_upper_left;
	t_v3 pixel00_loc;


} t_camera;



# define WINDOW_WIDTH 1920
# define WINDOW_HEIGHT 1080

// typedef struct s_vec3
// {
// 	float x;
// 	float y;
// 	float z;
// }				t_vec3;

// typedef struct s_color
// {
// 	float r;
// 	float g;
// 	float b;
// }				t_color;

typedef struct	s_ambient
{
	float	ratio;
	t_color	color;
}			t_ambient;

// typedef struct	s_camera
// {
// 	t_vec3	origin;
// 	t_vec3	direction;  //must be normalized (range [-1,1])
// 	float	fov; //Horizontal field of view in degrees in range [0,180]:
// }	t_camera;

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
	bool		is_valid;
}	t_scene;

typedef struct s_mini_rt
{
	mlx_t			*mlx;
	mlx_image_t		*image;
	t_scene			scene;
	t_camera		base_cam;
}	t_minirt;


/* ===================== INPUT PARSING & INIT ===================== */
int		validate_input(int argc, char *filename);
int		init_minirt(t_minirt *minirt, char **argv);
int		parse_file(t_minirt *minirt, char *filename);
int		parse_ambient(char **tokens, t_scene *scene);
int		parse_camera(char **tokens, t_scene *scene);
bool	check_comma_and_move(char **str, bool *is_valid);

/* ===================== Parse utils ===================== */
double	parse_float(const char *str, bool *is_valid);
t_color	parse_color(char *str, bool *is_valid);
t_vec3	parse_vec3(char *str, bool *is_valid);
bool	is_normalized_vec3(t_vec3 vector);

/* ===================== FOR DRAW ===================== */

/* ===================== FOR ERROR ===================== */
void	exit_error(const char *msg);

# endif
