/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_redir_lst.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otlacerd <otlacerd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 09:14:01 by olacerda          #+#    #+#             */
/*   Updated: 2026/03/16 22:13:43 by otlacerd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <data.h>

int	is_redirection(char *string)
{
	if (!string)
		return (-1);
	if (string_compare(string, ">") == 0)
		return (REDIR_OUTPUT);
	else if (string_compare(string, "<") == 0)
		return (REDIR_INPUT);
	else if (string_compare(string, ">>") == 0)
		return (REDIR_APPEND);
	else if (string_compare(string, "<<") == 0)
		return (REDIR_HEREDOC);
	return (0);
}

int	create_n_assign_node_redir(t_redir **head, char **splitted, int line)
{
	int	type;
	t_redir	*node;
	
	if (!head || !splitted)
		return (0);
	type = is_redirection(splitted[line]);
	node = NULL;
	if ((*head) == NULL)
	{
		(*head) = redir_lstnew(type, splitted[line + 1]);
		if (!(*head))
			return (0);
		node = (*head);
	}
	else
	{
		node = redir_lstnew(type, splitted[line + 1]);
		if (!node)
			return (redir_lstclear(&(*head), del), 0);
		redir_lstadd_back(&(*head), node);
	}
	return (1);
}

int	create_lst_redir(t_cmd *node, char **splitted, int line, int *heredoc_count)
{
	t_redir_type	type;
	// int				redir_nbr;

	if (!node || !splitted)
		return (0);
	// redir_nbr = 0;
	// (void)heredoc_count;
	while ((splitted[line] != NULL) && (splitted[line][0] != '|'))
	{
		type = is_redirection(splitted[line]);
		if ((type > 0))
		{
			if (create_n_assign_node_redir(&(node->redir), splitted, line)
				&& (type == REDIR_HEREDOC))
					(*heredoc_count)++;
				// redir_nbr++;
			free(splitted[line]);
			line += 2;
		}
		else
			line++;
	}
	return (line);
}
