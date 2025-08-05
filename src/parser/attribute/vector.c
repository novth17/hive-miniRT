#include "mini_rt.h"

static double	calc_v3_len(t_vec3 vec3)
{
	return (square_root(vec3.x * vec3.x + vec3.y * vec3.y + vec3.z * vec3.z));
}

bool is_normalized(t_vec3 vec)
{
    float len;

	len = calc_v3_len(vec);
	printf("Length of the vector is %f\n", len);
    return (fabs(len - 1.0f) < 1.1); //as long as len < 1.1 then it's ok
}

static double parse_vec3_comp(char **str, bool *is_valid_comp)
{
	char	temp[16];
	int		len;

	double	value;

	len = 0;
	*is_valid_comp = true;
	while ((*str)[len] && (*str)[len] != ',' && !ft_isspace((*str)[len]))
		len++;
	if (len == 0 || len > 15)
	{
		*is_valid_comp = false;
		return 0.0;
	}
	ft_strlcpy(temp, *str, len + 1);
	value = parse_float(temp, is_valid_comp);
	if (!*is_valid_comp)
		return 0.0;
	*str += len;
	return (value);
}

t_vec3 parse_vec3(char *str, bool *is_valid)
{
	t_vec3	vec3;

	*is_valid = true;

	vec3.x = parse_vec3_comp(&str, is_valid);
	if (!check_comma_and_move(&str, is_valid))
		return ((t_vec3){});

	vec3.y = parse_vec3_comp(&str, is_valid);
	if (!check_comma_and_move(&str, is_valid))
		return ((t_vec3){});

	vec3.z = parse_vec3_comp(&str, is_valid);
	if (!*is_valid || (*str != '\0' && !ft_isspace(*str)))
	{
		*is_valid = false;
		return ((t_vec3){});
	}
	return (vec3);
}
