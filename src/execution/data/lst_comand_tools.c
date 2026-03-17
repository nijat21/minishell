/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_comand_tools.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otlacerd <otlacerd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 22:45:35 by otlacerd          #+#    #+#             */
/*   Updated: 2026/03/17 00:22:29 by otlacerd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <data.h>

// void	comand_lstadd_back(t_cmd **lst, t_cmd *new)
// {
// 	t_cmd	*temp;

// 	temp = *lst;
// 	if (!new)
// 		return ;
// 	if (*lst == NULL)
// 	{
// 		*lst = new;
// 		new->next = NULL;
// 	}
// 	else
// 	{
// 		while (temp->next != NULL)
// 			temp = temp->next;
// 		temp->next = new;
// 		new->next = NULL;
// 	}
// }

// void	comand_lstclear(t_cmd **lst, void (*del)(void*))
// {
// 	t_cmd	*temp;
// 	t_cmd	*nexttemp;

// 	temp = *lst;
// 	while (temp != NULL)
// 	{
// 		nexttemp = temp->next;
// 		// del(temp->comand);
// 		free_array_string(temp->args, 0);
// 		redir_lstclear(&temp->redir, del);
// 		free(temp);
// 		temp = nexttemp;
// 	}
// 	*lst = NULL;
// }

// t_cmd	*comand_lstnew(char *comand, char **args)
// {
// 	t_cmd	*new;

// 	new = (t_cmd *) malloc(sizeof(t_cmd));
// 	if (!new)
// 		return (NULL);
// 	new->comand = comand;
// 	new->args = args;
// 	new->redir = NULL;
// 	new->next = NULL;
// 	return (new);
// }

int	comand_lstsize(t_cmd *lst)
{
	if (!lst)
		return (0);
	int	count;

	count = 1;
	while (lst->next != NULL)
	{
		lst = lst->next;
		count++;
	}
	return (count);
}