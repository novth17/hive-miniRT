/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_rt.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiennguy <hiennguy@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 20:30:17 by hiennguy          #+#    #+#             */
/*   Updated: 2025/07/24 21:58:22 by hiennguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_RT_H
#define MINI_RT_H

#define SUCCESS	0
#define FAIL	1

# include <fcntl.h>
# include "MLX42.h"
# include "libft.h"
# include "object.h"
# include "error.h"

# include "rt_math_hien.h"

# define WINDOW_WIDTH 800
# define WINDOW_HEIGHT 800

typedef struct	s_ambient
{
	float	ratio;
	t_color	color;
}			t_ambient;

typedef struct
{
	float		fov;
	t_point3	origin; // @QUESTION why this and camera_center??
	t_point3	direction;

	float	aspect_ratio; //
	int32_t image_width;
	int32_t image_height;
	int32_t samples_per_pixel;
	int32_t max_bounce; // for mandatory 1 or 2 i guess;

	t_v3		vup;

	float focal_length;
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

	uint32_t	spheres_count;
	t_sphere	*spheres;

	uint32_t	pl_count;
	t_plane		*pls;

	uint32_t	cyls_count;
	t_cylinder	*cyls;

	t_object	*objects;
	bool		is_valid;



}	t_scene;

typedef struct s_mini_rt
{
	mlx_t			*mlx;
	mlx_image_t		*image;

	t_scene			scene;
	char			**tokens;

}	t_minirt;


/* ===================== INPUT PARSING & INIT ===================== */
int		validate_input(int argc, char *filename);
int		init_minirt(t_minirt *minirt, char **argv);
int		parse_file(t_minirt *minirt, char *filename);
int		parse_ambient(char **tokens, t_scene *scene);
int		parse_camera(char **tokens, t_scene *scene);
int		parse_light(char **tokens, t_scene *scene);


int		parse_sphere(char **tokens, t_scene *scene);
int		parse_plane(char **tokens, t_scene *scene);
int		parse_cyl(char **tokens, t_scene *scene);



/* ===================== Parse utils ===================== */
double	parse_float(const char *str, bool *is_valid);
t_color	parse_color(char *str, bool *is_valid);
t_vec3	parse_vec3(char *str, bool *is_valid);
bool	is_normalized_vec3(t_vec3 vector);
bool	check_comma_and_move(char **str, bool *is_valid);

/* ===================== FOR DRAW ===================== */

/* ===================== FOR ERROR ===================== */
int		check_id_args_count(char **tokens, const char *id, int expected);
void	exit_error(const char *msg);
int		print_error(const char *msg, const char *value);

/* ===================== FOR DELETE ===================== */

void	delete_minirt(t_minirt *minirt);

# endif
