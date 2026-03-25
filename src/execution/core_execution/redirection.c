/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otlacerd <otlacerd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 12:22:13 by otlacerd          #+#    #+#             */
/*   Updated: 2026/03/25 06:30:49 by otlacerd         ###   ########.fr       */
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
	fd = open(redir->redir_arg, O_WRONLY | O_CREAT | O_TRUNC, 0644);
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
	fd = open(redir->redir_arg, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
		return (perror(redir->redir_arg), 0);
	dup2(fd, fds[1]);
	close(fd);
	return (1);
}

int	redir_heredoc(t_redir *redir, int fds[2], t_hdoc *heredoc)
{
	int		fd;
	char	**temps;

	int		fd;
	char	**temps;

	if (!fds || !redir || !heredoc || (heredoc->index >= heredoc->count))
		return (FAIL);
	temps = heredoc->temps;
	fd = open(temps[heredoc->index], O_RDONLY);
	if (fd < 0)
		return (perror(temps[heredoc->index]), 0);
	dup2(fd, fds[0]);
	close(fd);
	heredoc->index++;
	return (true);
}
