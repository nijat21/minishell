/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otlacerd <otlacerd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 12:15:59 by otlacerd          #+#    #+#             */
/*   Updated: 2026/03/19 07:02:55 by otlacerd         ###   ########.fr       */
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

int update_exit_status(int *exit_status, int status, int out_backup, int is_child)
{
	if (!exit_status)
    	return (0);
	if (is_child == false)
		*exit_status = status;
	else
	{
		if (WIFEXITED(status))
			*exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
		{
			*exit_status = 128 + WTERMSIG(status);
			if (*exit_status == 130)
				write(out_backup, "\n", 1);
		}
	}
    return (1);
}

int	wait_all_children(int *children_pids, int size, int *exit_status, int out_backup)
{
	int	is_child;
	int	index;
	int	status;

	if (!children_pids)
		return (0);
	index = 0;
	status = -1;
	is_child = true;
	while (index < size)
	{
		if (children_pids[index] > 1)
			(void)((waitpid(children_pids[index], &status, 0) == -1) && (perror("waitpid"), 0));
		else if (children_pids[index] <= 0)
		{
			status = -(children_pids[index]);
			if (index == (size - 1))
				is_child = false;
		}
		index++;
	}
	signals(false, 0);
	update_exit_status(exit_status, status, out_backup, is_child);
	return (1);
}

int	exec_fork(t_cmd *node, int node_nbr, t_origin *origin)
{
	int	pid;

	if (!node || (node_nbr < 0) || !origin)
		return (FAIL);
	pid = -1;
	if (has_next_comand(node) || (node_nbr > 0) || is_external_comand(origin))
	{
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			return (FAIL);
		}
		signals(true, pid);
		return (pid);
	}
	else
		return (NO_FORK);
}
