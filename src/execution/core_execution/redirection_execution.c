/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_execution.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nismayil <nismayil@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 07:54:47 by olacerda          #+#    #+#             */
/*   Updated: 2026/03/15 23:32:48 by nismayil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <core_execution.h>

int exec_heredoc(t_all *all, t_redir *redir, char **temps, int index)
{
	static int fd;
	int pid;
	int readbytes;

	if (!all || !redir)
		return (FAIL);
	fd = open(temps[index], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		return (0);
	pid = exec_heredoc_content(all, &(all->process_info->signal), redir, fd);
	(void)pid;
	close(fd);
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

int exec_all_heredocs(t_all *all)
{
	t_cmd *node;
	t_redir *redirection;
	int index;

	if (!all)
		return (0);
	node = all->head;
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
	// printf("ok\n");
	// exit(1);
	return (0);
}

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

int exec_redirections(t_all *all, t_cmd *node, int redir_fds[2], int pipe_fds[2])
{
	t_redir *redir;
	int result;

	if (!node || !pipe_fds || !redir_fds)
		return (0);
	redir = node->redir;
	result = 0;
	(void)all;
	while (redir != NULL)
	{
		sync_redir_n_pipe(node, redir, redir_fds, pipe_fds);
		if (redir->type == REDIR_OUTPUT)
			result = redir_out(redir, redir_fds);
		else if (redir->type == REDIR_INPUT)
			result = redir_in(redir, redir_fds);
		else if (redir->type == REDIR_APPEND)
			result = redir_append(redir, redir_fds);
		else if (redir->type == REDIR_HEREDOC)
			result = redir_heredoc(redir, redir_fds, all->heredoc_temps, all->heredoc_count);
		if (result != true)
			return (0);
		redir = redir->next;
	}
	return (1);
}
