
#include "mini_rt.h"


// maybe add this to some general utils file
static
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

// concatinate a number to the end of a t_string
bool cat_uint_to_str(t_string *dest, size_t num)
{
	const uint8_t	num_len = num_length(num);
	uint32_t		i;

	if (dest->len + num_len + 1 > dest->size)
		return (FAIL);
	dest->buf[dest->len + num_len] = '\0';
	i = num_len;
	while (i--)
	{
		dest->buf[dest->len + i] = (num % 10) + '0';
		num = num / 10;
	}
	dest->len += num_len;
	return (SUCCESS);
}

// concatinate a c string to the end of a t_string
bool cat_cstring_to_string(t_string *dest, char *cstr)
{
	size_t cstr_len;

	if (cstr == NULL)
		return (1);
	cstr_len = ft_strlen(cstr);
	if (dest->size <= dest->len + cstr_len)
		return (1);
	ft_memmove(&dest->buf[dest->len], cstr, cstr_len);
	dest->len += cstr_len;
	dest->buf[dest->len] = '\0';
	return (0);
}

