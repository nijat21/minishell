//
	/* ************************************************************************** */
//
	/*                                                                            */
//
	/*                                                        :::      ::::::::   */
//
	/*   create_redir_lst.c                                 :+:      :+:    :+:   */
// /*                                                    +:+ +:+        
	+:+     */
// /*   By: olacerda <olacerda@student.42.fr>          +#+  +:+      
	+#+        */
// /*                                                +#+#+#+#+#+  
	+#+           */
//
	/*   Created: 2026/03/04 09:14:01 by olacerda          #+#    #+#             */
//
	/*   Updated: 2026/03/11 13:20:48 by olacerda         ###   ########.fr       */
//
	/*                                                                            */
//
	/* ************************************************************************** */

#include <data.h>

int	comand_lstsize(t_cmd *lst)
{
	if (!lst)
		return (0);
	int count;

	count = 1;
	while (lst->next != NULL)
	{
		lst = lst->next;
		count++;
	}
	return (count);
}