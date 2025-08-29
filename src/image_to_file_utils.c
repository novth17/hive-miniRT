/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image_to_file_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiennguy <hiennguy@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 19:13:18 by ltaalas           #+#    #+#             */
/*   Updated: 2025/08/29 18:38:07 by hiennguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "image_to_file.h"

uint32_t	reorder_color(uint32_t color)
{
	const uint32_t	r = (color) & 0xFF;
	const uint32_t	g = (color >> 8) & 0xFF;
	const uint32_t	b = (color >> 16) & 0xFF;
	const uint32_t	a = (color >> 24) & 0xFF;

	return ((a << 24) | (r << 16) | (g << 8) | (b << 0));
}

uint8_t	num_length(uint32_t num)
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

uint32_t	blend_fore_and_background(uint32_t foreground, uint32_t background)
{
	const t_v4	foreground_linear = exact_unpack(foreground);
	const t_v4	background_linear = exact_unpack(background);
	const t_v3	foreground_color = foreground_linear.rgb;
	const t_v3	background_color = background_linear.rgb;
	t_v4		output_color;

	output_color.rgb = v3_lerp(background_color,
			foreground_linear.a,
			foreground_color);
	output_color.a = 1.0f;
	return (exact_pack(output_color));
}

t_bitmap_header	make_header(const mlx_image_t *image,
							uint32_t output_pixel_size)
{
	t_bitmap_header	header;

	header.file_type = 0x4D42;
	header.file_size = sizeof(header) + output_pixel_size;
	header.reserved1 = 0;
	header.reserved2 = 0;
	header.bitmap_offset = sizeof(header);
	header.size = sizeof(header) - 14;
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

void	apply_scene_file_name(t_string *filename, char *scene_name)
{
	uint32_t	scene_name_len_to_write;
	uint32_t	scene_name_len;
	char		*slash_position;

	slash_position = ft_strrchr(scene_name, '/');
	if (*slash_position == '/')
		scene_name = slash_position + 1;
	scene_name_len = ft_strlen(scene_name) - 3;
	scene_name_len_to_write = scene_name_len;
	if (scene_name_len_to_write > 20)
		scene_name_len_to_write = 20 - (sizeof("--truncated") - 1);
	cat_cstring_to_string_n(filename, scene_name, scene_name_len_to_write);
	cat_cstring_to_string(filename, "_rt");
	if (scene_name_len != scene_name_len_to_write)
		cat_cstring_to_string(filename, "--truncated");
}
