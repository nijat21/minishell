/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core_execution.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otlacerd <otlacerd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 04:34:05 by olacerda          #+#    #+#             */
/*   Updated: 2026/03/19 22:05:38 by otlacerd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <core_execution.h>

int get_cmd_origin(char **args, t_env *env, t_origin *origin, char *buffer)
{
	if (!origin || !args || !env)
		return (FAIL);
	origin->abs_path = NULL;
	origin->builtin = NULL;
	origin->builtin = get_built_in(args[0]);
	if (origin->builtin == NULL)
		origin->abs_path = get_absolute_path("PATH", args[0], env->envp, buffer);

	// int	line1 = -1;
	// while (args[++line1])	
	// 	dprintf(2, "args[%d] = %s\n", line1, args[line1]);
	
	update_underline_on_env(origin->abs_path, env, args); // decide if goes to final version
	return (1);
}

int exec_external_cmd(char *abs_path, char **args, t_all *all)
{
	if (!args || !all || !all->my_env || !all->my_env->envp || !all->fds)
		return (0);
	if (abs_path == NULL)
		end_structures(all, true, true, 0);	
	tcsetattr(all->fds->std_backup[0], TCSANOW, &(all->saved_termios));
	destroy_fds(all->fds, true);
	rl_clear_history();
	signals(true, CHILD);
	int line = -1;
	while (args && args[++line])
		if (!!execve(abs_path, args, all->my_env->envp))
		{
			restore_original_fds(all->fds);
			perror("execve");
		}
	handle_exit_status();
	end_structures(all, 1, 1, 1);
	return (1);
}

int exec_command(int node_nbr, t_cmd *node, t_origin *origin, t_all *all)
{
	int pid;

	if (!node || !origin || !all || !all->fds || !node->args[0])
		return (FAIL);
	pid = exec_fork(node, node_nbr, origin);
	if (pid > 0)
	{
		all->children_pids[node_nbr] = pid;
		return (0);
	}
	if (is_builtin(origin))
	{
		all->children_pids[node_nbr] = exec_builtin(origin, node, all);
		if (pid == CHILD)
			return (end_structures(all, true, true, -(all->children_pids[node_nbr])), 1);
	}
	else if ((origin->abs_path == NULL) || ((origin->abs_path)[0] == '\0'))
	{
		put_comand_error(node->args[0], "comand not found");
		all->children_pids[node_nbr] = -127;
	}
	if (pid == CHILD)
		exec_external_cmd(origin->abs_path, node->args, all);
	return (1);
}

int exec_linked_lst(t_all *all, t_cmd *node, t_fds *fds, t_env *env)
{
	t_origin origin;
	int redir_status;
	int node_nbr;

	if (!all || !node || !fds || !env)
		return (0);
	node_nbr = 0;
	while (node != NULL)
	{
		restore_original_fds(fds);
		get_pipe(fds, node);
		exec_redirections(all, node, fds, &redir_status);
		exec_pipe(fds->pipe);
		get_cmd_origin(node->args, env, &origin, all->buffer);
		if (redir_status == true)
			exec_command(node_nbr, node, &origin, all);
		close_pipe_fds(fds->pipe);
		node_nbr++;
		node = node->next;
	}
	return (1);
}

int exec_all_comands(t_all *all, t_cmd *node, char **envp)
{
	int size;
	if (!all || !node || !envp || (all->process_info->signal == SIGINT))
		return (FAIL);
	size = comand_lstsize(all->head);
	if (!size)
		return (0);
	create_children_pids_buffer(&all->children_pids, size);
	exec_linked_lst(all, node, all->fds, all->my_env);
	wait_all_children(all->children_pids, size, &(all->process_info->exit_status), all->fds->std_backup[1]);
	return (1);
}
