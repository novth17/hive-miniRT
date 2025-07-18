/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiennguy <hiennguy@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 17:49:03 by hiennguy          #+#    #+#             */
/*   Updated: 2024/11/12 14:18:12 by hiennguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/libft.h"

static int	create_and_add_node(t_list **new_list, void *(*f)(void *),
void (*del)(void *), t_list *lst)
{
	void	*content;
	t_list	*new_node;

	content = f(lst->content);
	if (content == NULL)
	{
		ft_lstclear(new_list, del);
		return (0);
	}
	new_node = ft_lstnew(content);
	if (new_node == NULL)
	{
		del(content);
		ft_lstclear(new_list, del);
		return (0);
	}
	ft_lstadd_back(new_list, new_node);
	return (1);
}

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new_list;

	new_list = NULL;
	if (f == NULL || del == NULL)
		return (NULL);
	while (lst)
	{
		if (create_and_add_node(&new_list, f, del, lst) == 0)
			return (NULL);
		lst = lst->next;
	}
	return (new_list);
}
