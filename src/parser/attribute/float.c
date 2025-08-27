#include "mini_rt.h"

static int	check_decimal_places(const char *str);
static int	is_valid_float(const char *str, int i, int dot_count);

double	parse_float(const char *str, bool *is_valid)
{
	double	value;
	int		i;
	int		dot_count;

	i = 0;
	dot_count = 0;
	if (!is_valid_float(str, i, dot_count))
	{
		*is_valid = false;
		return (0.0);
	}
	value = ft_atof(str);
	if (value < -2147483648.0 || value > 2147483647.0)
	{
		*is_valid = false;
		return (0.0);
	}
	*is_valid = true;
	return (value);
}

static int	is_valid_float(const char *str, int i, int dot_count)
{
	if (!str || !*str)
		return (0);
	if (str[i] == '-' || str[i] == '+')
		i++;
	if (!ft_isdigit(str[i]))
		return (0);
	if (check_decimal_places(str) == FAIL)
		return (0);
	while (str[i])
	{
		if (str[i] == '.')
		{
			dot_count++;
			if (dot_count > 1 || !ft_isdigit(str[i + 1]))
				return (0);
		}
		else if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

static int	check_decimal_places(const char *str)
{
	int	decimal_places;

	decimal_places = 0;
	while (*str && *str != '.')
		str++;
	if (*str == '.')
		str++;
	while (*str && ft_isdigit(*str))
	{
		decimal_places++;
		if (decimal_places > 7)
			return (FAIL);
		str++;
	}
	return (SUCCESS);
}
