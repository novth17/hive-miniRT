#include "../inc/mini_rt.h"
#include <fcntl.h>
#include <stdint.h>
#include <errno.h>
#include "../inc/image_to_file.h"

void	convert_mlx_image_into_output_image(uint32_t *output,
											const mlx_image_t *image,
											const uint32_t *foreground_pixels,
											const uint32_t *background_pixels)
{
	const t_cord	lim = {.x = image->width, .y = image->height};
	uint32_t		background;
	uint32_t		color;
	uint32_t		x;
	uint32_t		y;

	y = 0;
	while (y < lim.y)
	{
		x = 0;
		while (x < lim.x)
		{
			color = foreground_pixels[((lim.y - 1 - y) * lim.x) + x];
			if (background_pixels != NULL)
			{
				background = background_pixels[((lim.y - 1 - y) * lim.x) + x];
				color = blend_fore_and_background(color, background);
			}
			*output++ = reorder_color(color);
			x++;
		}
		y++;
	}
}

static
int	write_image(const mlx_image_t *image, mlx_image_t *background, int fd)
{
	const uint32_t	*foreground_pixels = (uint32_t *)image->pixels;
	const uint32_t	output_size = image->width * image->height * 4;
	t_bitmap_header	header;
	uint32_t		*output_image;
	uint32_t		*background_pixels;

	if (fd == -1)
		return (1);
	output_image = malloc(output_size);
	if (output_image == NULL)
	{
		print_error("miniRT: Failed to allocate for output image", NULL);
		return (1);
	}
	background_pixels = NULL;
	if (background != NULL)
		background_pixels = (uint32_t *)background->pixels;
	convert_mlx_image_into_output_image(output_image, image,
		foreground_pixels, background_pixels);
	header = make_header(image, output_size);
	write(fd, &header, sizeof(header));
	write(fd, output_image, output_size);
	close(fd);
	free(output_image);
	return (0);
}

static
void	add_num_and_extension_to_filename(t_string *filename, uint32_t num)
{
	uint32_t	len_required;
	int			error;

	error = 0;
	if (num > 0)
	{
		error += cat_cstring_to_string(filename, "(");
		error += cat_uint_to_str(filename, num);
		error += cat_cstring_to_string(filename, ")");
	}
	error += cat_cstring_to_string(filename, OUTPUT_FILE_EXTENSION);
	if (error)
	{
		len_required = sizeof(OUTPUT_FILE_EXTENSION);
		len_required += (sizeof("--truncated") - 1);
		if (num > 0)
			len_required += num_length(num) + 2;
		filename->len -= len_required;
		cat_cstring_to_string(filename, "--truncated");
		add_num_and_extension_to_filename(filename, num);
	}
}

static
void	create_filename_base(t_string *filename, t_minirt *minirt)
{
	static const uint32_t	filename_start_length = sizeof(OUTPUT_FILENAME) - 1;
	static char				name_buf[FILENAME_BUFFER_SIZE] = OUTPUT_FILENAME;
	const char				*scene_name = minirt->scene_file_name;
	const uint32_t			scene_name_len = ft_strlen(scene_name) - 3;
	uint32_t				scene_name_len_to_write;

	filename->buf = name_buf;
	filename->len = filename_start_length;
	filename->size = sizeof(name_buf);
	scene_name_len_to_write = scene_name_len;
	if (scene_name_len_to_write > 20)
		scene_name_len_to_write = 17;
	cat_cstring_to_string_n(filename, scene_name, scene_name_len_to_write);
	if (scene_name_len != scene_name_len_to_write)
		cat_cstring_to_string(filename, "...");
	cat_cstring_to_string(filename, "_rt_");
	cat_cstring_to_string(filename, "_spp-");
	cat_uint_to_str(filename, minirt->scene.camera.samples_per_pixel);
	cat_cstring_to_string(filename, "_maxbounces-");
	cat_uint_to_str(filename, minirt->scene.camera.max_bounce);
	cat_cstring_to_string(filename, "_frames-");
	cat_uint_to_str(filename, minirt->accumulated_frames);
	cat_cstring_to_string(filename, "_avg_frame_time-");
	cat_uint_to_str(filename, minirt->avg_frame_time);
	cat_cstring_to_string(filename, "ms");
}

void	pixels_to_image_file(t_minirt *minirt)
{
	uint32_t		num;
	t_string		filename;
	int				fd;
	int				tries;

	create_filename_base(&filename, minirt);
	num = 0;
	tries = 0;
	fd = -1;
	while (fd == -1 && tries < 1000)
	{
		add_num_and_extension_to_filename(&filename, num);
		fd = open(filename.buf, O_CREAT | O_EXCL | O_TRUNC | O_RDWR, 0644);
		if (fd == -1 && errno != EEXIST)
		{
			print_error("Failed to create image file", strerror(errno));
			return ;
		}
		++tries;
		++num;
	}
	if (fd == -1)
		print_error("Failed to create image file after 1000 tries", NULL);
	write_image(minirt->image, minirt->background, fd);
}
