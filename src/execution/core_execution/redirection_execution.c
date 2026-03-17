/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_execution.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otlacerd <otlacerd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 07:54:47 by olacerda          #+#    #+#             */
/*   Updated: 2026/03/17 00:49:19 by otlacerd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <core_execution.h>

int	exec_heredoc(t_all *all, t_redir *redir, char **temps, int index)
{
	static int	fd;
	// int			pid;
	int			readbytes;
 
	if (!all || !redir)
		return (FAIL);
	fd = open(temps[index], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		return (0);
	exec_heredoc_content(all, &(all->process_info->signal), redir->redir_arg, fd);
	// (void)pid;
	close (fd);
	fd = open(temps[index], O_RDONLY);
	readbytes = read(fd, all->buffer, BUFFER_SIZE);
	close(fd);
	if (readbytes != 0)
	{
		realloc_appender(&all->main_line, "\n");
		add_heredoc_history(all->buffer, all->main_line, string_length(all->main_line), temps[index]);
	}
	return (true);
}

int	count_heredocs(t_cmd *head)
{
	t_cmd *node;
	t_redir *redir;
	int	count;

	if (!head)
		return (0);
	count = 0;
	node = head;
	while (node != NULL)
	{
		redir = node->redir;
		while (redir != NULL)
		{
			if (redir->type == REDIR_HEREDOC)
				count++;
			redir = redir->next;
		}
		node = node->next;
	}
	return (count);	
}


int	exec_all_heredocs(t_all *all)
{
	t_cmd	*node;
	t_redir *redirection;
	int		index;

	if (!all)
		return (0);
	node = all->head;
	all->heredoc_count = count_heredocs(all->head);
	all->heredoc_temps = create_heredoc_temps_buffer(all->heredoc_count, all->father_pid);
	index = 0;
	while (node != NULL)
	{
		redirection = node->redir;
		while (redirection != NULL)
		{
			if (redirection->type == REDIR_HEREDOC)
			{
				exec_heredoc(all, redirection, all->heredoc_temps, index);
				index++;
			}
			redirection = redirection->next;
		}
		node = node->next;
	}
	return (0);
}

int	sync_redir_n_pipe(t_cmd *node, t_redir *redir, int *red_fd, int *pp_fd)
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
