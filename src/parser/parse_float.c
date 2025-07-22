/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_float.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiennguy <hiennguy@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 19:10:50 by hiennguy          #+#    #+#             */
/*   Updated: 2025/07/22 21:28:59 by hiennguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "mini_rt.h"

static int	is_valid_float(const char *str);

double parse_float(const char *str, bool *flag)
{
	double number;

	number = 0.0;
	if (is_valid_float(str) == 0)
	{
		flag = false;
		ft_dprintf(2, "Error: Wrong float format!\n");
		return (NAN);
	}
	if (*flag == true)
		number = ft_atof(str);
	return (number);
}

static int	is_valid_format(const char *str)
{
	int	i;
	int	dot_count;

	dot_count = 0;
	i = 0;
	if (!str || !*str)
		return (0);
	if (str[i] == '-' || str[i] == '+')
		i++;
	if (!ft_isdigit(str[i]))
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

static int	is_valid_float(const char *str)
{
	double	value;
	int		decimal_places;

	decimal_places = 0;
	if (!is_valid_format(str))
		return (0);
	value = ft_atof(str);
	if (value < -1.0 || value > 1.0)
		return (0);
	while (*str && *str != '.')
		str++;
	if (*str == '.')
		str++;
	while (*str && ft_isdigit(*str))
	{
		decimal_places++;
		if (decimal_places > 8)
			return (0);
		str++;
	}
	return (1);
}

