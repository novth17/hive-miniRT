/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiennguy <hiennguy@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 20:54:52 by hiennguy          #+#    #+#             */
/*   Updated: 2025/07/21 20:59:37 by hiennguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "mini_rt.h"

int validate_input(int argc, char *filename)
{
	size_t len;

	if (argc != 2)
	{
		ft_dprintf(2, "Error: Argument must be 1 as a file [fname].rt!\n");
		return (FAIL);
	}
	len = ft_strlen(filename);
	if (len <= 3)
	{
		ft_dprintf(2, "Error: Wrong file name format ");
		ft_dprintf(2,"| Usage: ./mini_rt [fname].rt\n");
		return (FAIL);
	}
	if (ft_strcmp(filename + len - 3, ".rt") != 0)
	{
		ft_dprintf(2, "Error: File must end with .rt ");
		ft_dprintf(2, "| Usage: ./mini_rt [fname].rt\n");
		return (FAIL);
	}
	return (SUCCESS);
}
