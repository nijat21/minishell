/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otlacerd <otlacerd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 12:15:59 by otlacerd          #+#    #+#             */
/*   Updated: 2026/03/25 06:08:35 by otlacerd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <core_execution.h>

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

void	check_status(int status)
{
	int	sig;
	int	sig;

	sig = WTERMSIG(status);
	if (WIFSIGNALED(status))
	{
		if (sig == SIGSEGV)
			put_error("Segmentation fault");
		else if (sig == SIGABRT)
			put_error("Aborted");
		else if (sig == SIGFPE)
			put_error("Floating point exception");
		else if (sig == SIGBUS)
			put_error("Bus error");
		else if (sig == SIGILL)
			put_error("Killed");
		else if (sig == SIGQUIT)
			put_error("Quit");
		if (WCOREDUMP(status))
		if (WCOREDUMP(status))
			put_error(" (core dumped)");
		put_error("\n");
	}
}

int	update_exit_status(int *exit_status, int status, int out_bkup, int is_child)
{
	if (!exit_status)
		return (0);
		return (0);
	if (is_child == false)
		*exit_status = status;
	else
	{
		if (WIFEXITED(status))
			*exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
		{
			check_status(status);
			*exit_status = 128 + WTERMSIG(status);
			(void) out_bkup;
		}
	}
	return (1);
	return (1);
}

int	wait_all_children(int *buffer, int size, int *exit_stat, int out_bkup)
{
	int	is_child;
	int	index;
	int	status;

	if (!buffer || (size == 0))
		return (0);
	index = -1;
	is_child = true;
	status = 0;
	while (++index < size)
	{
		if (buffer[index] > 1)
		{
			if (waitpid(buffer[index], &status, 0) == -1)
				perror("waitpid");
		}
		else if (buffer[index] <= 0)
		{
			status = -(buffer[index]);
			if (index == (size - 1))
				is_child = false;
		}
	}
	signals(false, 0);
	return (update_exit_status(exit_stat, status, out_bkup, is_child), 1);
}

int	exec_fork(t_cmd *node, int node_nbr, t_origin *origin)
{
	int	pid;
	int	pid;

	if (!node || (node_nbr < 0) || !origin)
		return (FAIL);
	pid = -1;
	if (!((origin->abs_path == NULL) && (origin->builtin == NULL))
		&& (has_next_comand(node, origin)
			|| (node_nbr > 0) || is_external_comand(origin)))
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
