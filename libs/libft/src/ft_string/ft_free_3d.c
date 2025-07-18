/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_3d.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiennguy <hiennguy@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 11:29:06 by hiennguy          #+#    #+#             */
/*   Updated: 2025/02/14 11:29:23 by hiennguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/libft.h"

void	ft_free_3d(char ***all_words)
{
	int	i;

	i = 0;
	while (all_words[i] != NULL)
	{
		free(all_words[i]);
		i++;
	}
	free(all_words);
}
