/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_execution.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otlacerd <otlacerd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 07:54:47 by olacerda          #+#    #+#             */
/*   Updated: 2026/03/17 21:49:37 by otlacerd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <core_execution.h>

int sync_redir_n_pipe(t_cmd *node, t_redir *redir, int *red_fd, int *pp_fd)
{
	if (!node || !red_fd || !pp_fd || !redir)
		return (0);
	if ((node->next != NULL) && (redir->type == REDIR_OUTPUT || redir->type == REDIR_APPEND))
		red_fd[1] = pp_fd[1];
	if ((pp_fd[0] != -1) && (redir->type == REDIR_HEREDOC || redir->type == REDIR_INPUT))
		red_fd[0] = pp_fd[0];
	return (1);
}

int	exec_redirections(t_all *all, t_cmd *node, t_fds *fds, int *redir_status)
{
	t_redir	*redir;
	int	result;
	
	if (!node || !fds)
		return (FAIL);
	redir = node->redir;
	result = 0;
	while (redir != NULL)
	{
		sync_redir_n_pipe(node, redir, fds->redir, fds->pipe);
		if (redir->type == REDIR_OUTPUT)
			result = redir_out(redir, fds->redir);	
		else if (redir->type == REDIR_INPUT)
			result = redir_in(redir, fds->redir);
		else if (redir->type == REDIR_APPEND)
			result = redir_append(redir, fds->redir);
		else if (redir->type == REDIR_HEREDOC)
			result = redir_heredoc(redir, fds->redir, all->heredoc_temps, all->heredoc_count);
		if (result != true)
			return ((*redir_status) = false, 0);
		redir = redir->next;
	}
	return ((*redir_status) = true, 1);
}

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
