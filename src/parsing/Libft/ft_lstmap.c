/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nismayil <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 13:19:30 by nismayil          #+#    #+#             */
/*   Updated: 2024/11/21 13:19:34 by nismayil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static t_list	*create_add_node(t_list *lst, void *(*f)(void *),
		t_list **new_list, t_list *curr_node)
{
	void	*new_cont;
	t_list	*new_node;

	new_cont = f(lst->content);
	if (!new_cont)
		return (NULL);
	new_node = ft_lstnew(new_cont);
	if (!new_node)
		return (NULL);
	if (!*new_list)
		*new_list = new_node;
	else
		curr_node->next = new_node;
	return (new_node);
}

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new_list;
	t_list	*new_node;
	t_list	*curr_node;

	if (!lst || !f || !del)
		return (NULL);
	new_list = NULL;
	while (lst)
	{
		new_node = create_add_node(lst, f, &new_list, curr_node);
		if (!new_node)
		{
			ft_lstclear(&new_list, del);
			return (NULL);
		}
		curr_node = new_node;
		lst = lst->next;
	}
	return (new_list);
}
