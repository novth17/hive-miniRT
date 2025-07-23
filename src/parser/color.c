/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiennguy <hiennguy@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 14:47:29 by hiennguy          #+#    #+#             */
/*   Updated: 2025/07/23 16:01:28 by hiennguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

static int parse_rgb_comp(char **tokens, char **str)
{
	long	num;
	int		i;

	num = 0;
	i = 0;
	if (!ft_isdigit(**str))
	{
		ft_free_2d(tokens);
		exit_error("Color: Component must start with a digit\n");
	}
	while (ft_isdigit((*str)[i]))
	{
		num = num * 10 + (*str)[i] - '0';
		if (num > 255)
			break ;
		i++;
	}
	if (i == 0 || num > 255)
	{
		ft_free_2d(tokens);
		exit_error("Color: Component must start with a digit2\n");
	}
	*str += i;
	return ((int)num);
}

static void	check_comma(char **tokens, char *str, char *color)
{
	if (*str != ',')
	{
		ft_dprintf(2, "At: %s\n", str);
		ft_free_2d(tokens);
		ft_dprintf(2, "Color: Expected ',' after the %s color\n", color);
		exit(FAIL);
	}
}
t_color parse_color(char **tokens, char *str)
{
	t_color	color;

	color.r = parse_rgb_comp(tokens, &str);
	check_comma(tokens, str, "red");
	str++;

	color.g = parse_rgb_comp(tokens, &str);
	check_comma(tokens, str, "green");
	str++;

	color.b = parse_rgb_comp(tokens, &str);
	if (*str != '\0' && !ft_isspace(*str))
	{
		ft_dprintf(2, "At: %s\n", str);
		ft_free_2d(tokens);
		exit_error("Color: Must be end of file or a space at the end");
	}
	return (color);
}
