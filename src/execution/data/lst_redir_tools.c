/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_redir_tool.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olacerda <olacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 05:09:11 by otlacerd          #+#    #+#             */
/*   Updated: 2026/03/04 09:45:45 by olacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <data.h>

void	redir_lstadd_back(t_redir **lst, t_redir *new)
{
	t_redir	*temp;

	temp = *lst;
	if (!new)
	{
		printf("erro na protecao");
		return ;
	}
	if (*lst == NULL)
	{
		*lst = new;
		new->next = NULL;
	}
	else
	{
		while (temp->next != NULL)
			temp = temp->next;
		temp->next = new;
		new->next = NULL;
	}
}

void	redir_lstclear(t_redir **lst, void (*del)(void*))
{
	t_redir	*temp;
	t_redir	*nexttemp;

	temp = *lst;
	while (temp != NULL)
	{
		nexttemp = temp->next;
		del(temp->redir_arg);
		free(temp);
		temp = nexttemp;
	}
	*lst = NULL;
}

t_redir	*redir_lstnew(t_redir_type type, char *arg)
{
	t_redir	*new;

	new = (t_redir *) malloc(sizeof(t_redir));
	if (!new)
		return (NULL);
	new->type = type;
	new->redir_arg = arg;
	new->next = NULL;
	return (new);
}

int	redir_lstsize(t_redir *lst)
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