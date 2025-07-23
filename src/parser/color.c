/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiennguy <hiennguy@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 14:47:29 by hiennguy          #+#    #+#             */
/*   Updated: 2025/07/23 20:27:03 by hiennguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

static int parse_rgb_comp(char **str, bool *is_valid_comp)
{
	long	num;
	int		i;

	num = 0;
	i = 0;
	if (!ft_isdigit(**str))
	{
		*is_valid_comp = false;
		return (0);
	}
	while ((*str)[i] && ft_isdigit((*str)[i]))
	{
		num = num * 10 + (*str)[i] - '0';
		if (num > 255)
			break ;
		i++;
	}
	if (i == 0 || num > 255)
	{
		*is_valid_comp = false;
		return (0);
	}
	*str += i;
	return ((int)num);
}

t_color parse_color(char *str, bool *is_valid)
{
	t_color	color;

	*is_valid = true;

	color.r = parse_rgb_comp(&str, is_valid);
	if (!check_comma_and_move(&str, is_valid))
		return ((t_color){0, 0, 0});

	color.g = parse_rgb_comp(&str, is_valid);
	if (!check_comma_and_move(&str, is_valid))
		return ((t_color){0, 0, 0});

	color.b = parse_rgb_comp(&str, is_valid);
	if (!*is_valid || (*str != '\0' && !ft_isspace(*str)))
	{
		*is_valid = false;
		return ((t_color){0, 0, 0});
	}
	return color;
}

