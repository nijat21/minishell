/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otlacerd <otlacerd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 07:04:23 by olacerda          #+#    #+#             */
/*   Updated: 2026/03/17 00:06:03 by otlacerd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <core_execution.h>

int	save_original_fds(int *std_backup)
{
	if (!std_backup)
		return (0);
	std_backup[0] = dup(STDIN_FILENO);
	std_backup[1] = dup(STDOUT_FILENO);
	return (1);
}

int	copy_fds(int fds1[2], int fds2[2])
{
	if (!fds1 || !fds2)
		return (0);
	fds1[0] = fds2[0];
	fds1[1] = fds2[1];
	return (1);
}

int	destroy_fds(t_fds *fds, int flag)
{
	if (!fds)
		return (0);
	safe_close_fd(&fds->pipe[0]);
	safe_close_fd(&fds->pipe[1]);
	safe_close_fd(&fds->redir[0]);
	safe_close_fd(&fds->redir[1]);
	safe_close_fd(&fds->previous_0);
	if (flag == 1)
	{
		safe_close_fd(&fds->std_backup[0]);
		safe_close_fd(&fds->std_backup[1]);
	}
	return (1);
}

int	restore_original_fds(t_fds *fds)
{
	if (!fds)
		return (0);
	fds->redir[0] = STDIN_FILENO;
	fds->redir[1] = STDOUT_FILENO;
	fds->pipe[1] = -1;
	fds->pipe[0] = -1;
	dup2(fds->std_backup[0], fds->redir[0]);
	dup2(fds->std_backup[1], fds->redir[1]);
	return (1);
}

int	safe_close_fd(int *fd)
{
	if (!fd)
		return (0);
	if ((*fd >= 0) && !((*fd == STDIN_FILENO) || (*fd == STDOUT_FILENO)
				|| (*fd == STDERR_FILENO)))
	{
		close (*fd);
	}
	*fd = -1;
	return (1);
}

int	close_pipe_fds(int *pipe_fds)
{
	if ((pipe_fds[0] >= 0) && (isatty(pipe_fds[0]) == false))
		safe_close_fd(&(pipe_fds[0]));
	if ((pipe_fds[1] >= 0) && (isatty(pipe_fds[1]) == false))
		safe_close_fd(&(pipe_fds[1]));
	return (1);
}