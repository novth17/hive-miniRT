/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiennguy <hiennguy@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 20:30:30 by hiennguy          #+#    #+#             */
/*   Updated: 2025/07/15 20:45:09 by hiennguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

int	main(int argc, char **argv)
{
	(void) **argv;
	if (argc != 2)
	{
		write(1, "oh no, try again\n", 17);
		return (FAIL);
	}
	write(1, "hihihaha\n", 8);
	return (SUCCESS);
}
