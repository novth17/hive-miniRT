# include "../inc/mini_rt.h"
#include <math.h>

#include <stdbool.h>
#include <stdint.h>
#include "../inc/image_to_file.h"


static uint32_t g_accummulated_frames = 0;

static inline
bool shadow_hit(const t_scene *scene, const t_ray ray, const float light_distance)
{
	uint32_t i;
	float hit_distance;

	i = -1;
	while (++i < scene->pl_count)
	{
		hit_distance = plane_hit(scene->pls[i], ray);
		if (hit_distance > MIN_HIT_DIST && hit_distance < light_distance)
			return (true);
	}
	i = -1;
	while (++i < scene->spheres_count)
	{
		hit_distance = sphere_hit(scene->spheres[i], ray);
		if(hit_distance > MIN_HIT_DIST && hit_distance < light_distance)// sphere_hit(&temp_rec, scene->spheres[i], ray))
			return (true);
	}
	i = -1;
	while (++i < scene->cyls_count)
	{
		hit_distance = cyl_hit(scene->cyls[i], ray);
		if(hit_distance > MIN_HIT_DIST && hit_distance < MAX_HIT_DIST)// sphere_hit(&temp_rec, scene->spheres[i], ray))
			return (true);
	}
	return (false);
}

static inline
t_v3 point_light_color(const t_light *restrict light, const t_hit *restrict rec, t_v3 light_direction, float dist)
{
	// look at https://en.wikipedia.org/wiki/Blinn%E2%80%93Phong_reflection_model to make this better
	// const t_color ambient_light = f32_mul_v3(scene->ambient.ratio, scene->ambient.color);
	const t_v3 point_light_color = f32_mul_v3(light->bright_ratio, light->color);
	float light_angle;
	t_v3 color;

	// light_direction = f32_mul_v3(1.0f / dist, light_direction);
	light_angle = smoothstep(dot(rec->normal, light_direction), 0.0f, 2.0f);
	color = f32_mul_v3(light_angle, point_light_color);
	color = f32_mul_v3(1.0f / (dist * dist), color);
	// color = V3_ADD(ambient_light, color);
	// color = v3_mul_v3(rec->color, color);
	return (v3_clamp(color));
}

static inline
t_v3 check_point_light(const t_scene *restrict scene, const t_hit *restrict rec)
{
	uint32_t	i;
	t_color	color;
	t_ray shadow_ray;
	t_v3 light_vector;
	float light_distance;

	i = 0;
	color = v3(0, 0, 0);
	while (i < scene->lights_count)
	{
		light_vector = V3_SUB(scene->lights[i].origin, rec->position);
		light_distance = length(light_vector);
		shadow_ray.origin = rec->position;
		shadow_ray.direction = f32_mul_v3(1.0f / light_distance, light_vector);
		if (shadow_hit(scene, shadow_ray, light_distance) == false)
		{
			color = V3_ADD(color, point_light_color(&scene->lights[i], rec, shadow_ray.direction, light_distance / scene->light_dist_mult));
		}
		i++;
	}
	return (v3_clamp(color));
}


static inline
bool find_closest_ray_intesection(t_hit *restrict rec, const t_ray ray, const t_scene * restrict scene)
{
	// t_sphere point_light_sphere = {.material.color = v3(1, 1, 1), .center = scene->light.origin, .radius = 0.05f}; // debugging
	// point_light_sphere.material.emitter = 0.0f;
	// point_light_sphere.material.diffuse = 0.0f;
	// point_light_sphere.material.specular_probability = 0.0f;

	check_planes(rec, scene->pls, scene->pl_count, ray);
	check_spheres(rec, scene->spheres, scene->spheres_count, ray);
	// check_spheres(&hit_record, &point_light_sphere, 1, ray);
	check_cyl(rec, scene->cyls, scene->cyls_count, ray);

	return (rec->did_hit);
}



#define HIT_ONCE 0
#define PREV_SPECULAR 1

static inline
t_v4 trace(t_ray ray, const t_scene * restrict scene, const uint32_t max_bounce, uint32_t *seed) // change to all objects or scene;
{
	t_v3 ambient = f32_mul_v3(scene->ambient.ratio, scene->ambient.color);
	// t_v3 point_light_color = f32_mul_v3(scene->light.bright_ratio * scene->light_strength_mult, scene->light.color);
	// t_sphere point_light_sphere = {.color = v3(20, 20, 20), .center = scene->light.origin, .radius = 0.05f};
	uint32_t i;
	t_color total_incoming_light;


	t_hit rec;
	rec = (t_hit){};

	t_color ray_color;
	t_color prev_color;
	ray_color = v3(1, 1, 1);
	prev_color = ray_color;
	total_incoming_light = v3(0, 0, 0);
	bool hit_once = false;
	bool prev_specular = true;

	i = 0;
	while (i <= max_bounce)
	{
		rec.distance = MAX_HIT_DIST;
		if (find_closest_ray_intesection(&rec, ray, scene))
		{
			// if we hit something calculate the light contribution of that point into the total light of the ray

			hit_once = true;
			rec.position = V3_ADD(rec.position, v3_mul_f32(rec.normal, 1e-4f));
			// rec.position = V3_SUB(rec->position, v3_mul_f32(ray.direction, 1e-4f)); // look to see if this value is good or not
			const bool is_specular_bounce = rec.mat.specular_probability >= random_float(seed);
			t_color emmitted_light = v3_mul_f32(rec.mat.color, rec.mat.emitter);
			total_incoming_light = V3_ADD(total_incoming_light, v3_mul_v3(emmitted_light, ray_color));
			ray_color = v3_mul_v3(ray_color, v3_lerp(rec.mat.color, is_specular_bounce, rec.mat.specular_color));
			if (scene->use_point_light)
			{
				total_incoming_light = V3_ADD(total_incoming_light, v3_mul_v3(check_point_light(scene, &rec), ray_color));
			}

			if (i > 0 && !prev_specular)
			{
				float p = fmaxf(ray_color.r, fmaxf(ray_color.g, ray_color.b));
				if (random_float(seed) >= p)
				{
					ray_color = prev_color;
					break;
				}
			}
			prev_specular = is_specular_bounce;
			prev_color = ray_color;
			ray = calculate_next_ray(&rec, ray, is_specular_bounce, seed);
			++i;
		}
		else
		{

			break ;
		}
	}
	if (hit_once)
	{
		total_incoming_light = V3_ADD(total_incoming_light, v3_mul_v3(ambient, ray_color));
		return ((t_v4){.rgb = v3_clamp(total_incoming_light), .a = 1.0f});
	}

	return (v4(0, 0, 0, 0));
}

t_v4 sample_pixel(const t_scene *scene, const t_camera *restrict cam, const t_cord original_cord, uint32_t seed)
{
	t_ray	ray;
	t_v4	color;
	t_v4	incoming_light;
	int32_t	y_s;
	int32_t	x_s;

	incoming_light = v4(0, 0, 0, 0);
	y_s = 0;
	while (y_s < cam->sqrt_spp)
	{
		x_s = 0;
		while (x_s < cam->sqrt_spp)
		{

			ray = get_ray(cam, original_cord, (t_cord){x_s, y_s}, &seed);
			color = trace(ray, scene, cam->max_bounce, &seed);
			incoming_light = v4_add(incoming_light, color);
			++x_s;
		}
		++y_s;
	}
	color = v4_mul_f32(incoming_light, cam->pixel_sample_scale_strati);
	return (color);
}

// static inline
// t_v3 rgb_u32_to_float(uint32_t c)
// {
// 	t_v3 result;

// 	result.b = (float)((c >> 16) & 0xFF) / 255.0f;
// 	result.g = (float)((c >> 8) & 0xFF) / 255.0f;
// 	result.r = (float)((c >> 0) & 0xFF) / 255.0f;
// 	return (result);
// }

static inline
t_v4 accumulate(const t_v4 old_color, const t_v4 new_color)
{
	const float weight = 1.0 / (g_accummulated_frames + 1); // @TODO screw around with the weight
	t_v4 accumulated_average;

	accumulated_average.r = old_color.r * (1 - weight) + new_color.r * weight;
	accumulated_average.g = old_color.g * (1 - weight) + new_color.g * weight;
	accumulated_average.b = old_color.b * (1 - weight) + new_color.b * weight;
	accumulated_average.a = old_color.a * (1 - weight) + new_color.a * weight;
	return (accumulated_average);
}



void render_tile(t_task task)
{
	t_v4		color;
	uint32_t	*out;
	uint32_t	x;
	uint32_t	y;
	uint32_t	rng_seed;


	y = task.y_start;
	while (y < task.y_end_plus_one)
	{
		out = task.out + (y * task.cam->image_width);
		x = task.x_start;
		while (x < task.x_end_plus_one)
		{
			rng_seed = (y * task.cam->image_width + x) + g_accummulated_frames * 792858;
			color = sample_pixel(task.scene, task.cam, (t_cord){x, y}, rng_seed);

			color = accumulate(exact_unpack(out[x]), color);
			out[x] = exact_pack(color);





			++x;
		}
		++y;
	}
	// return (0);
}

static
void recalculate_camera(t_minirt *minirt, t_camera *frame_cam)
{
	init_camera_for_frame(minirt, &minirt->scene.camera);
	*frame_cam = minirt->scene.camera;
	minirt->recalculate_cam = false;
	g_accummulated_frames = 0;
}



void prepare_to_render(t_minirt *minirt, mlx_t *mlx, mlx_image_t *img, t_camera *frame_cam)
{
	set_title(minirt);
	if (minirt->write_image_to_file == true)
	{
		pixels_to_image_file(minirt);
		minirt->write_image_to_file = false;
	}
	if (check_movement_keys(&minirt->scene.camera, mlx, mlx->delta_time))
		minirt->recalculate_cam = true;
	mouse_control(minirt);
	if (img->width != (uint32_t)mlx->width || img->height != (uint32_t)mlx->height)
	{
		if (mlx_resize_image(minirt->image, mlx->width, mlx->height) == false)
			ft_putstr_fd("miniRT: Failed to resize image\n", 2);
		if (mlx_resize_image(minirt->background, mlx->width, mlx->height) == false)
			ft_putstr_fd("miniRT: Failed to resize background\n", 2);
		recalculate_camera(minirt, frame_cam);
		draw_background(minirt);
		if (minirt->queue.tasks && minirt->queue.tasks != get_default_task())
			free(minirt->queue.tasks);
		create_task_queue(&minirt->queue, minirt, frame_cam);
	}
	if (minirt->recalculate_cam == true)
	{
		recalculate_camera(minirt, frame_cam);
	}
}



#ifdef MINIRT_BONUS


bool get_and_render_tile(t_task_queue *queue)
{
	t_task *task;

	uint32_t work_order_index;

	work_order_index = atomic_fetch_add(&queue->next_task_index, 1);
	if (work_order_index >= queue->task_count)
	{
		return (false);
	}
	task = queue->tasks + work_order_index;
	render_tile(*task);
	atomic_fetch_add(&queue->tiles_retired_count, 1);

	return (true);
}

void per_frame(void * param)
{
	t_camera *frame_cam;
	t_minirt *minirt;
	mlx_t *mlx;
	t_task_queue *queue;

	minirt = (t_minirt *)param;
	mlx = minirt->mlx;
	frame_cam = get_frame_cam();
	prepare_to_render(minirt, mlx, minirt->image, frame_cam);
	queue = &minirt->queue;
	queue->tiles_retired_count = 0;
	queue->next_task_index = 0;
	atomic_exchange(&minirt->render, true);
	while (queue->tiles_retired_count < queue->task_count)
	{
		usleep(1000);
	}
	atomic_exchange(&minirt->render, false);
	++g_accummulated_frames;
}

#else

bool	get_and_render_tile(t_task_queue *queue)
{
	const t_task *task = queue->tasks;

	render_tile(*task);
	queue->tiles_retired_count++;
	return (false);
}

void per_frame(void * param)
{
	t_camera		*frame_cam;
	t_minirt		*minirt;
	mlx_t			*mlx;
	t_task_queue	*queue;

	minirt = (t_minirt *)param;
	mlx = minirt->mlx;
	frame_cam = get_frame_cam();
	prepare_to_render(minirt, mlx, minirt->image, frame_cam);
	queue = &minirt->queue;
	queue->tiles_retired_count = 0;
	queue->next_task_index = 0;
	render_tile(queue->tasks[0]);
	++g_accummulated_frames;
}

#endif



// luminance and brightness threshold stuff
//
// lerp(color.r)

			// uint32_t rgba = exact_pack(color);
			// int num = roundf(random_float(&rng_seed) * 4.0f);
			// uint32_t r = (rgba >> 0) & 0xff;
			// uint32_t g = (rgba >> 8) & 0xff;
			// uint32_t b = (rgba >> 16) & 0xff;
			// uint32_t a = (rgba >> 24) & 0xff;
			// switch (num)
			// {
			// 	case 1: r += random_float(&rng_seed) * 4.0f; break;
			// 	case 2: g += random_float(&rng_seed) * 4.0f; break;
			// 	case 3: b += random_float(&rng_seed) * 4.0f; break;
			// }

			// *out++ = (a << 24 | b << 16 | g << 8 | r << 0);
			// t_color luminance;
		 // 	luminance.r = 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
			// luminance.g = 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
			// luminance.b = 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
			// t_color threshold;

			// threshold = (t_v3){};
			// if (luminance.r >= 0.5f)
			// 	threshold.r = 1.0f;
			// if (luminance.g >= 0.5f)
			// 	threshold.g = 1.0f;
			// if (luminance.b >= 0.5f)
			// 	threshold.b = 1.0f;

			// *out++ = exact_pack((t_v4){.rgb = threshold, .a = color.a});
			// *out++ = exact_pack((t_v4){.rgb = luminance, .a = color.a});


			// color = accumulate(gamma2_unpack(*out), color);
			// *out++ = gamma2_pack(color);


			// color = accumulate(rgb_u32_to_v4(*out), color);
			// *out++ = rgb_pack4x8(v3_clamp(color.rgb)); // maybe dont need clamp or do it in color correction
