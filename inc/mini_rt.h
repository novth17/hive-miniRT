/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_rt.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiennguy <hiennguy@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 20:30:17 by hiennguy          #+#    #+#             */
/*   Updated: 2025/07/18 23:47:27 by hiennguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef MINI_RT_H
#define MINI_RT_H

#define SUCCESS	0
#define FAIL	1

# include "MLX42.h"
# include "libft.h"

#include "../test_stuff/rt_math.h"

# define WINDOW_WIDTH 800
# define WINDOW_HEIGHT 800

typedef	struct	s_vec3 {
	double x;
	double y;
	double z;
}				t_vec3;


typedef struct s_mini_rt
{
	mlx_t		*mlx;
	mlx_image_t	*image;


	t_vec3		cam;

}				t_mini_rt;

/* ===================== INPUT PARSING & INIT ===================== */
void	init_minirt(t_mini_rt *minirt, char **argv);


/* ===================== FOR DRAW ===================== */


#endif
