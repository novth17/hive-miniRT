/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image_to_file.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 16:29:49 by ltaalas           #+#    #+#             */
/*   Updated: 2025/08/29 16:29:51 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IMAGE_TO_FILE_H
# define IMAGE_TO_FILE_H

# include <string.h>
# include "mini_rt.h"
# include "MLX42.h"

# define OUTPUT_FILENAME "minirt_"
# define FILENAME_BUFFER_SIZE 80
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

void			pixels_to_image_file(t_minirt *minirt);

// utils
uint32_t		reorder_color(uint32_t color);
uint8_t			num_length(uint32_t num);
uint32_t		blend_fore_and_background(uint32_t foreground,
					uint32_t background);
t_bitmap_header	make_header(const mlx_image_t *image,
					uint32_t output_pixel_size);

#endif
