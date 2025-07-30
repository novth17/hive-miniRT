#include "../inc/mini_rt.h"
#include <fcntl.h>
#include <stdint.h>
#include <errno.h>
#include "../inc/image_to_file.h"

static
t_bitmap_header make_header(mlx_image_t *image)
{
	const uint32_t output_pixel_size = image->width * image->height * sizeof(uint32_t);

	t_bitmap_header header;
	header.file_type = 0x4D42;
	header.file_size = sizeof(header) + output_pixel_size;
	header.bitmap_offset = sizeof(header);
	header.size = sizeof(header) - 14; // full header - sizeof 1st 5 elements
	header.width = image->width;
	header.height = image->height;
	header.color_planes = 1;
	header.bits_per_pixel = 32;
	header.compression = 0;
	header.size_of_bitmap = output_pixel_size;
	header.horizontal_ppm = 0;
	header.vertical_ppm = 0;
	header.colors_used = 0;
	header.colors_important = 0;
	return (header);
}

uint32_t reorder_color(uint32_t color)
{
	const uint32_t r = (color) & 0xFF;
	const uint32_t g = (color >> 8) & 0xFF;
	const uint32_t b = (color >> 16) & 0xFF;
	const uint32_t a = (color >> 24) & 0xFF;

	return ((a << 24) | (r << 16) | (g << 8) | (b << 0));

}

uint32_t	*convert_mlx_image_to_output_image(mlx_image_t *image, uint32_t output_pixel_size)
{
	const uint32_t *src = (uint32_t *)image->pixels;
	uint32_t *pixels;
	uint32_t color;
	size_t x;
	size_t y;

	pixels = malloc(output_pixel_size);
	if (pixels == NULL)
	{
		print_error("Failed to allocate for output image", NULL);
		return (NULL);
	}
	y = 0;
	while (y < image->height)
	{
		x = 0;
		while (x < image->width)
		{
			color = src[((image->height - 1 - y )  * image->width) + x];
			pixels[y * image->width + x] = reorder_color(color);
			x++;
		}
		y++;
	}
	return (pixels);
}

static
int write_image(mlx_image_t *image, int fd)
{
	const uint32_t output_pixel_size = image->width * image->height * sizeof(uint32_t);
	const t_bitmap_header header = make_header(image);
	uint32_t *output_image;

	if (fd == -1)
		return (1);
	output_image = convert_mlx_image_to_output_image(image, output_pixel_size);
	if (output_image == NULL)
		return (1);
	write(fd, &header, sizeof(header));
	write(fd, output_image, output_pixel_size);
	close(fd);
	free(output_image);
	return (0);
}

static
uint8_t	num_len(uint32_t num)
{
	uint8_t	i;

	i = 0;
	if (num == 0)
		return (1);
	while (num > 0)
	{
		num = num / 10;
		i++;
	}
	return (i);
}

#define OUTPUT_FILENAME "minirt_img"
#define OUTPUT_FILENAME_BUFFER_SIZE 30

static
char	*create_file_name(uint32_t num)
{
	static const uint32_t filename_length = sizeof(OUTPUT_FILENAME) - 1;
	static char name_buf[OUTPUT_FILENAME_BUFFER_SIZE] = OUTPUT_FILENAME;
	uint8_t		i;

	i = num_len(num);
	name_buf[filename_length + i] = '\0';
	while (i--)
	{
		name_buf[filename_length + i] = (num % 10) + '0';
		num = num / 10;
	}
	return (name_buf);
}

void pixels_to_image_file(mlx_image_t *image)
{
	static const char extension[] = ".bmp";
	uint32_t num;
	char *filename;
	int fd;

	num = 1;
	fd = -1;
	while (fd == -1)
	{
		filename = create_file_name(num);
		ft_strlcat(filename, extension, OUTPUT_FILENAME_BUFFER_SIZE);
		fd = open(filename, O_CREAT | O_EXCL | O_TRUNC | O_RDWR, 0644);
		if (fd == -1 && errno != EEXIST)
		{
			print_error("Failed to create image file", NULL);
			return ;
		}
		++num;
	}
	write_image(image, fd);
}
