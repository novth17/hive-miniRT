#ifndef IMAGE_TO_FILE_H
# define IMAGE_TO_FILE_H

# include <stdint.h>
# include "MLX42.h"

# define OUTPUT_FILENAME "minirt_img"
# define OUTPUT_FILENAME_BUFFER_SIZE 30
# define OUTPUT_FILE_EXTENSION ".bmp"


# pragma pack(push, 1)

typedef struct s_bitmap_header
{
	//header header
	uint16_t	file_type;
	uint32_t	file_size;
	uint16_t	reserved1;
	uint16_t	reserved2;
	uint32_t	bitmap_offset;
	//info_header
	uint32_t	size;
	uint32_t	width;
	uint32_t	height;
	uint16_t	color_planes;
	uint16_t	bits_per_pixel;
	uint32_t	compression;
	uint32_t	size_of_bitmap;
	int32_t		horizontal_ppm;
	int32_t		vertical_ppm;
	uint32_t	colors_used;
	uint32_t	colors_important;
}	t_bitmap_header;

# pragma pack(pop)

void	pixels_to_image_file(mlx_image_t *image);

// utils
uint32_t	reorder_color(uint32_t color);
uint8_t	num_len(uint32_t num);


#endif
