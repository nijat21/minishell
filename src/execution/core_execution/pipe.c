/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otlacerd <otlacerd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 03:35:41 by olacerda          #+#    #+#             */
/*   Updated: 2026/03/12 18:52:19 by otlacerd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <core_execution.h>

int	exec_pipe(int *fds)
{
	if (!fds)
		return (0);
	if ((fds[0] >= 0) && (isatty(fds[0]) == false))
	{
		dup2(fds[0], STDIN_FILENO);
		close(fds[0]);
	}
	if ((fds[1] >= 0) && (isatty(fds[1]) == false))
	{
		dup2(fds[1], STDOUT_FILENO);
		close(fds[1]);
	}
	return (1);
}

int	get_pipe(t_fds *fds, t_cmd *node)
{
	static int	temp_fd_0 = -1;
	
	if (!fds || !node)
		return (0);
	if (node->next != NULL)
	{
		if (pipe(fds->pipe) == -1)
			return (put_error("Error\nFailed pipe in node_execution\n"), 0);
		// *temp = fds->pipe[0];
		temp_fd_0 = fds->pipe[0];
	}
	fds->pipe[0] = fds->previous_0;
	fds->previous_0 = temp_fd_0;
	return (1);
}
