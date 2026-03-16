/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nismayil <nismayil@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 19:09:02 by nismayil          #+#    #+#             */
/*   Updated: 2026/03/16 15:52:05 by nismayil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <lexer.h>

void free_seg_list(t_seg **seg)
{
	t_seg *temp;

	while (*seg)
	{
		temp = (*seg)->next;
		free((*seg)->val);
		free((*seg));
		(*seg) = temp;
	}
	*seg = NULL;
}

void free_token_list(t_token **tk)
{
	t_token *temp;

	while (*tk)
	{
		temp = (*tk)->next;
		free_seg_list(&(*tk)->seg_list);
		free((*tk));
		(*tk) = temp;
	}
	*tk = NULL;
}

void *safe_malloc(size_t bytes, char *func_name)
{
	void *mem;

	mem = malloc(bytes);
	if (!mem)
	{
		ft_putstr_fd("malloc: ", STDERR_FILENO);
		ft_putstr_fd(func_name, STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
		return (NULL);
	}
	return (mem);
}

bool is_space(char c)
{
	if (c == ' ' || c == '\n' || c == '\t' || c == '\r' || c == '\v' || c == '\f')
		return (true);
	else
		return (false);
}

bool is_operator(const char c)
{
	return (c == '|' || c == '<' || c == '>');
}
