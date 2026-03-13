/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pid.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olacerda <olacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 12:15:59 by otlacerd          #+#    #+#             */
/*   Updated: 2026/03/10 22:10:14 by olacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <core_execution.h>

void handle_exit_status(void)
{
    if (errno == ENOENT)
        exit (127);
	else if (errno == EACCES)
		exit (126);
    exit(1);
}

int	create_children_pids_buffer(int **children_pids, int size)
{
	int	index;

	if (!children_pids || !size)
		return (0);
	index = 0;
	*children_pids = malloc(size * sizeof(int));
	while (index < size)
	{
		(*children_pids)[index] = -1;
		index++;
	}
	return (1);
}

int	update_exit_status(int *exit_status, int status)
{
	if (!exit_status)
		return (0);
	if (WIFEXITED(status))
		(*exit_status) = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		(*exit_status) = 128 + WTERMSIG(status);
	return (1);
}

int	wait_all_children(int *children_pids, int size, int *exit_status)
{
	int	check_wait;
	int	index;
	int	status;

	if (!children_pids)
		return (0);
	index = 0;
	check_wait = 0;
	status = -1;
	while (index < size)
	{
		if (children_pids[index] > 1)
		{
			check_wait = waitpid(children_pids[index], &status, 0);
			if (check_wait == -1)
				perror("waitpid");
		}
		else if (children_pids[index] < 0)
			status = -(children_pids[index]);
		index++;
	}
	update_exit_status(exit_status, status);
	return (1);
}


