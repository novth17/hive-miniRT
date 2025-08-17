
#include <stdint.h>

uint32_t reorder_color(uint32_t color)
{
	const uint32_t r = (color) & 0xFF;
	const uint32_t g = (color >> 8) & 0xFF;
	const uint32_t b = (color >> 16) & 0xFF;
	const uint32_t a = (color >> 24) & 0xFF;

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
