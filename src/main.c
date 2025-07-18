/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiennguy <hiennguy@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 22:28:07 by hiennguy          #+#    #+#             */
/*   Updated: 2025/07/18 22:35:47 by hiennguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "mini_rt.h"


static void	run_minirt(char **argv)
{
	t_mini_rt minirt;

	init_minirt(&minirt, argv);
	//parse(&minirt, argv[1]);
	//render(&minirt);
	mlx_loop(minirt.mlx);
	mlx_terminate(minirt.mlx);
}

int	main(int argc, char **argv)
{
	if (argc != 2)
	{
		write(1, "oh no, try again\n", 17);
		return (FAIL);
	}
	run_minirt(argv);
	write(1, "hihihaha\n", 8);
	return (SUCCESS);
}


