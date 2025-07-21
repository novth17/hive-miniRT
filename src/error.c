/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiennguy <hiennguy@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 15:26:29 by hiennguy          #+#    #+#             */
/*   Updated: 2025/07/21 15:50:11 by hiennguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "mini_rt.h"

void	exit_error(const char *msg)
{
	ft_dprintf(2, "Error\n%s\n", msg);
	exit(FAIL);
}


int	print_ret_error(const char *msg)
{
	ft_dprintf(2, "Error\n%s\n", msg);
	return(FAIL);
}
