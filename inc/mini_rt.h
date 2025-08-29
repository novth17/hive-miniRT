/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_rt.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiennguy <hiennguy@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 23:08:09 by hiennguy          #+#    #+#             */
/*   Updated: 2025/08/29 14:22:44 by hiennguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_RT_H
# define MINI_RT_H

# define SUCCESS	0
# define FAIL	1

# include <fcntl.h>

# include "MLX42.h"
# include "types.h"
# include "libft.h"
# include "object.h"
# include "error.h"
# include "hit.h"

# include "rt_math.h"
# include "camera.h"
# include "task.h"
# include "thread.h"

# ifdef MINIRT_BONUS
#  include <stdatomic.h>
# endif

# define WINDOW_WIDTH 1080
# define WINDOW_HEIGHT 1080

# define MIN_HIT_DIST 0.0001f
# define MAX_HIT_DIST FLT_MAX

typedef struct s_ambient
{
	float	ratio;
	t_color	color;
}	t_ambient;

typedef struct s_scene
{
	t_ambient	ambient;
	t_camera	camera;
	t_cylinder	*cyls;
	t_object	*objects;
	t_light		*lights;
	t_sphere	*spheres;
	t_plane		*pls;
	uint32_t	lights_count;
	uint32_t	spheres_count;
	uint32_t	pl_count;
	uint32_t	cyls_count;
	uint32_t	camera_count;
	uint32_t	ambient_count;
	float		light_dist_mult;
	bool		use_point_light;
	bool		is_valid;

}	t_scene;

# ifdef MINIRT_BONUS

typedef struct s_minirt
{
	mlx_t					*mlx;
	t_v4					*linear_color_buf;
	mlx_image_t				*image;
	mlx_image_t				*background;
	char					*scene_file_name;

	size_t					accumulated_frames;
	double					accum_start_time;
	double					total_frame_time;
	double					avg_frame_time;
	t_scene					scene;

	t_task_queue			queue;
	int						core_count;
	bool					file_has_content;
	bool					has_camera;
	bool					write_image_to_file;
	bool					recalculate_cam;
	volatile atomic_bool	stop_threads;
	volatile atomic_bool	render;

}	t_minirt;

# else

typedef struct s_minirt
{
	mlx_t			*mlx;
	t_v4			*linear_color_buf;
	mlx_image_t		*image;
	mlx_image_t		*background;
	char			*scene_file_name;

	size_t			accumulated_frames;
	double			accum_start_time;
	double			total_frame_time;
	double			avg_frame_time;
	t_scene			scene;

	t_task_queue	queue;
	int				core_count;
	bool			file_has_content;
	bool			has_camera;
	bool			write_image_to_file;
	bool			recalculate_cam;
	volatile bool	stop_threads;
	volatile bool	render;

}	t_minirt;

# endif

typedef struct s_string
{
	char	*buf;
	size_t	len;
	size_t	size;
}	t_string;

// getter.c
t_camera	*get_frame_cam(void);

// main.c
int			resize_linear_color_buf(t_minirt *minirt);

/* ============== Helpers for string type and title function ============== */
// title.c
bool		cat_cstring_to_string(t_string *dest, const char *cstr);
bool		cat_cstring_to_string_n(t_string *dest, const char *cstr,
				uint32_t n);
bool		cat_uint_to_str(t_string *dest, size_t num);
void		set_title(t_minirt *minirt);

/* ===================== INPUT PARSING & INIT ===================== */
int			validate_input(int argc, char *filename);
void		print_guide(void);

int			init_minirt(t_minirt *minirt, char **argv);
int			parse_file(t_minirt *minirt, char *filename);
int			parse_ambient(char **tokens, t_scene *scene);
int			parse_camera(char **tokens, t_scene *scene);
int			parse_light(char **tokens, t_scene *scene);

int			parse_sphere(char **tokens, t_scene *scene);
int			parse_plane(char **tokens, t_scene *scene);
int			parse_cyl(char **tokens, t_scene *scene);

/* ===================== Parse utils ===================== */
double		parse_float(const char *str, bool *is_valid);
t_color		parse_color(char *str, bool *is_valid);
t_vec3		parse_vec3(char *str, bool *is_valid);
bool		is_in_range_vec3(t_vec3 vector);
bool		check_comma_and_move(char **str, bool *is_valid);
bool		is_normalized(t_vec3 *vec);
int			parse_material(char **t, t_scene *scene, t_material *m, int o);
char		*set_next_line(char **line, int fd);
void		check_fd(t_minirt *minirt, int fd);
void		normalize_whitespace(char *line);

/* ===================== FILL ARRAY ===================== */
int			fill_obj_arr(t_minirt *minirt, t_scene *scene);
t_object	*create_objects(t_scene *scene, t_obj_type type);

/* ===================== FOR CONTROL ===================== */

void		set_in_bounds(t_camera *cam);
bool		should_recalculate(keys_t key);
bool		check_movement_keys(t_camera *cam, mlx_t *mlx, double time_step);
void		key_hook(struct mlx_key_data data, void *param);
void		scroll_hook(double delta_x, double delta_y, void *param);
void		mouse_control(t_minirt *minirt);
void		rotate_view(t_camera *cam);
void		reset_rotation_angles(void);

/* ===================== FOR DRAW ===================== */
// ray.c
t_ray		get_ray(const t_camera *restrict cam,
				t_cord cord, t_cord strati, uint32_t *seed);
t_ray		calculate_next_ray(const t_hit *restrict rec,
				t_ray ray, bool is_specular_bounce, uint32_t *seed);
bool		find_closest_ray_intesection(
				t_hit *restrict rec,
				const t_ray ray,
				const t_scene *restrict scene);

void		init_camera_for_frame(t_minirt *minirt, t_camera *cam);
void		base_init_cam(t_camera *cam);

// prepare_to_render.c
void		prepare_to_render(t_minirt *minirt, mlx_t *mlx,
				mlx_image_t *img, t_camera *frame_cam);

// render.c
void		render_tile(t_task task);
void		recalculate_camera(t_minirt *minirt, t_camera *frame_cam);
void		per_frame(void *param);

//sample.c
t_v4		sample_pixel(const t_scene *scene, const t_camera *restrict cam,
				const t_cord original_cord, uint32_t seed);

// point_light.c
t_v3		check_point_light(const t_scene *restrict scene,
				const t_hit *restrict rec);

/* ===================== FOR CHECK HIT ===================== */
float		check_planes(t_hit *restrict rec, const t_plane *planes,
				const uint32_t count, const t_ray ray);
float		plane_hit(const t_plane pl, const t_ray ray);
float		check_cyl(t_hit *restrict rec, const t_cylinder *cyl,
				const uint32_t count, const t_ray ray);
float		cyl_hit(const t_cylinder cyl, const t_ray ray);
float		check_spheres(t_hit *restrict rec, const t_sphere *spheres,
				const uint32_t count, const t_ray ray);
float		sphere_hit(const t_sphere sp, const t_ray ray);

t_color		checker_plane(t_v3 p, const t_plane *pl);
t_color		checker_sphere(t_v3 p, const t_sphere *sphere);
t_color		checker_cylinder(t_v3 p, const t_cylinder *cyl);

/* ===================== FOR ERROR ===================== */
int			check_id_args_count(char **tokens, const char *id, int expected);
void		exit_error(t_minirt *minirt, char *msg);
int			print_error(const char *msg, const char *value);
int			check_obj_args_count(char **tokens, const char *id,
				int min, int max);

/* ===================== FOR DELETE ===================== */

void		delete_minirt(t_minirt *minirt);

/* ===================== FOR BACKGROUND ===================== */
int			init_background(t_minirt *minirt);
void		draw_background(t_minirt *mrt);

#endif
