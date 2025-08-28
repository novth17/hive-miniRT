/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiennguy <hiennguy@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 17:22:11 by hiennguy          #+#    #+#             */
/*   Updated: 2025/08/28 17:22:13 by hiennguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

static float	parse_rgb_comp(char **str, bool *is_valid_comp)
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
	return (exact_srgb_to_linear((float)num / 255.0f));
}

t_color	parse_color(char *str, bool *is_valid)
{
	t_color	color;

	*is_valid = true;
	color.r = parse_rgb_comp(&str, is_valid);
	if (!check_comma_and_move(&str, is_valid))
		return ((t_color){});
	color.g = parse_rgb_comp(&str, is_valid);
	if (!check_comma_and_move(&str, is_valid))
		return ((t_color){});
	color.b = parse_rgb_comp(&str, is_valid);
	if (!*is_valid || (*str != '\0' && !ft_isspace(*str)))
	{
		*is_valid = false;
		return ((t_color){});
	}
	return (color);
}
