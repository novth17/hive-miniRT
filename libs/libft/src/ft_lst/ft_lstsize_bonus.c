/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiennguy <hiennguy@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 17:50:37 by hiennguy          #+#    #+#             */
/*   Updated: 2024/11/11 17:51:28 by hiennguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/libft.h"

int	ft_lstsize(t_list *lst)
{
	t_list	*current_node;
	int		count;

	current_node = lst;
	count = 0;
	while (current_node != NULL)
	{
		current_node = current_node->next;
		count++;
	}
	return (count);
}
