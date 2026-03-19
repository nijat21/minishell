/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otlacerd <otlacerd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 12:22:13 by otlacerd          #+#    #+#             */
/*   Updated: 2026/03/19 03:16:19 by otlacerd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <core_execution.h>

int	redir_in(t_redir *redir, int fds[2])
{
	int	fd;

	if (!redir || !fds)
		return (-1);
	fd = open(redir->redir_arg, O_RDONLY);
	if (fd < 0)
		return (perror(redir->redir_arg), 0);
	dup2(fd, fds[0]);
	close(fd);
	return (1);
}

int	redir_out(t_redir *redir, int fds[2])
{
	int	fd;

	if (!redir || !fds)
		return (-1);
	fd = open (redir->redir_arg, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		return (perror(redir->redir_arg), 0);
	dup2(fd, fds[1]);
	close(fd);
	return (1);
}

int	redir_append(t_redir *redir, int fds[2])
{
	int	fd;

	if (!redir || !fds)
		return (-1);
	fd = open (redir->redir_arg, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
		return (perror(redir->redir_arg), 0);
	dup2(fd, fds[1]);
	close(fd);
	return (1);
}

int	redir_heredoc(t_redir *redir, int fds[2], char **temps, int count)
{
	static int	fd;
	static int	index;
 
	if (!fds || !redir)
		return (FAIL);
	if (index >= count)
		index = 0;
	fd = open(temps[index], O_RDONLY);
	if (fd < 0)
		return (perror(temps[index]), 0);
	dup2(fd, fds[0]);
	close(fd);
	index++;
	return (true);
}
