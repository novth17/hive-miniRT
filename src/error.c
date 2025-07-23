/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiennguy <hiennguy@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 15:26:29 by hiennguy          #+#    #+#             */
/*   Updated: 2025/07/23 21:51:33 by hiennguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "mini_rt.h"

void	exit_error(const char *msg)
{
	ft_dprintf(2, "Error: %s\n", msg);
	exit(FAIL);
}

int print_error(const char *msg, const char *value)
{
	if (value)
		ft_dprintf(2, "%s: %s\n", msg, value);
	else
		ft_dprintf(2, "%s\n", msg);
	return (FAIL);
}

int check_id_args_count(char **tokens, const char *id, int expected)
{
	if (ft_count_2d(tokens) != expected)
	{
		ft_dprintf(2, "%s: Number of arguments must be %d!\n", id, expected);
		return (FAIL);
	}
	return (SUCCESS);
}
