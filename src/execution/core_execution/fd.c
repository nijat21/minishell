/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otlacerd <otlacerd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 07:04:23 by olacerda          #+#    #+#             */
/*   Updated: 2026/03/23 12:46:42 by otlacerd         ###   ########.fr       */
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

int	destroy_fds(t_fds *fds, int is_the_end)
{
	if (!fds)
		return (0);
	safe_close_fd(&fds->pipe[0]);
	safe_close_fd(&fds->pipe[1]);
	safe_close_fd(&fds->previous_0);
	if (is_the_end == 1)
	{
		safe_close_fd(&fds->dev_null);
		safe_close_fd(&fds->std_backup[0]);
		safe_close_fd(&fds->std_backup[1]);
	}
	return (1);
}

int	restore_fds(t_fds *fds, t_cmd *node)
{
	if (!fds)
		return (0);
	safe_close_fd(&fds->pipe[0]);
	safe_close_fd(&fds->pipe[1]);
	fds->pipe[1] = -1;
	fds->pipe[0] = -1;
	safe_close_fd(&fds->io[0]);
	safe_close_fd(&fds->io[1]);
	fds->io[0] = STDIN_FILENO;
	fds->io[1] = STDOUT_FILENO;
	dup2(fds->std_backup[0], fds->io[0]);
	dup2(fds->std_backup[1], fds->io[1]);
	if (node && (node->next == NULL))
		safe_close_fd(&fds->previous_0);
	return (1);
}

int safe_close_fd(int *fd)
{
	if (!fd || (*fd < 0))
		return (-1);
	if ((*fd == STDIN_FILENO || *fd == STDOUT_FILENO || *fd == STDERR_FILENO))
	{
		if (isatty(*fd) == true)
			return (0);
	}
	close (*fd);
	*fd = -1;
	return (1);
}

int	close_pipe_fds(int *pipe_fds)
{
	if ((pipe_fds[0] < 0) || (pipe_fds[1] < 0))
		return (0);
	if (isatty(pipe_fds[0]) == false)
		safe_close_fd(&(pipe_fds[0]));
	if (isatty(pipe_fds[1]) == false)
		safe_close_fd(&(pipe_fds[1]));
	return (1);
}