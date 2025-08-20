#ifndef MINI_RT_H
#define MINI_RT_H

#include "types.h"
#define SUCCESS	0
#define FAIL	1

#include <stdio.h> // @WARNING FOR DEBUGGING CURRENTLY

# include <fcntl.h>
# include "../libs/MLX42/include/MLX42/MLX42.h"
# include "../libs/libft/inc/libft.h"
# include "object.h"
# include "error.h"
# include "hit.h"

# include "rt_math.h"
# include "camera.h"

# define WINDOW_WIDTH 1080
# define WINDOW_HEIGHT 1080


#define MIN_HIT_DIST 0.0001f
#define MAX_HIT_DIST FLT_MAX // for now

typedef struct	s_ambient
{
	float	ratio;
	t_color	color;
}			t_ambient;


// typedef struct	s_light
// {
// 	t_vec3	origin;
// 	float	bright_ratio; // Brightness ratio in range [0.0, 1.0]
// 	t_color	color;
// }	t_light;

typedef struct s_scene
{

	t_ambient	ambient;
	t_camera	camera;
	t_light		light;

	float		light_dist_mult;
	bool		use_point_light;

	uint32_t	lights_count;
	t_light		*lights;

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
	mlx_image_t		*background;

	t_scene			scene;
	bool			file_has_content;
	bool			has_camera;

	bool			write_image_to_file;
	bool			recalculate_cam;
}	t_minirt;

typedef struct s_string
{
	char *buf;
	size_t len;
	size_t size;
}	t_string;

/* ===================== Helpers for string type ===================== */
// string.c
bool cat_cstring_to_string(t_string *dest, char *cstr);
bool cat_uint_to_str(t_string *dest, size_t num);


/* ===================== INPUT PARSING & INIT ===================== */
int		validate_input(int argc, char *filename);
void	print_guide(void);

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
bool	is_in_range_vec3(t_vec3 vector);
bool	check_comma_and_move(char **str, bool *is_valid);
bool	is_normalized(t_vec3 *vec);
int		parse_material(char **t, t_scene *scene, t_material *m, int o);
char	*set_next_line(char **line, int fd);
void	check_fd(t_minirt *minirt, int fd);
void	normalize_whitespace(char *line);

/* ===================== FILL ARRAY ===================== */
int fill_obj_arr(t_minirt *minirt, t_scene *scene);
t_object	*create_objects(t_scene *scene, t_obj_type type);

/* ===================== FOR CONTROL ===================== */

void set_in_bounds(t_camera *cam);
bool should_recalculate(keys_t key);
bool check_movement_keys(t_camera *cam, mlx_t *mlx, double time_step);
void key_hook(struct mlx_key_data data, void * param);
void scroll_hook(double delta_x, double delta_y, void *param);
void mouse_control(t_minirt *minirt);
void rotate_view(t_camera *cam);

/* ===================== FOR DRAW ===================== */
void init_camera_for_frame(t_minirt *minirt, t_camera *cam);
void base_init_cam(t_camera *cam);

void per_frame(void * param);

/* ===================== FOR CHECK HIT ===================== */
float check_planes(t_hit *restrict rec, const t_plane *planes, const uint32_t count, const t_ray ray);
float plane_hit(const t_plane pl, const t_ray ray);
float check_cyl(t_hit *restrict rec, const t_cylinder *cyl, const uint32_t count, const t_ray ray);
float cyl_hit(const t_cylinder cyl, const t_ray ray);
float	check_spheres(t_hit *restrict rec, const t_sphere *spheres, const uint32_t count, const t_ray ray);
float	sphere_hit(const t_sphere sp, const t_ray ray);

/* ===================== FOR ERROR ===================== */
int		check_id_args_count(char **tokens, const char *id, int expected);
void	exit_error(t_minirt *minirt,  char *msg);
int		print_error(const char *msg, const char *value);
int		check_obj_args_count(char **tokens, const char *id, int min, int max);

/* ===================== FOR DELETE ===================== */

void	delete_minirt(t_minirt *minirt);


/* ===================== FOR BACKGROUND ===================== */
int		init_background(t_minirt *minirt);
void	draw_background(t_minirt *mrt);


# endif
