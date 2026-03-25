/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core_execution.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otlacerd <otlacerd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 04:34:05 by olacerda          #+#    #+#             */
/*   Updated: 2026/03/25 01:32:49 by otlacerd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <core_execution.h>

int	validate_absolute_path(char *comand)
{
	struct stat	st;

	if (!comand)
		return (0);
	if (access(comand, F_OK) != 0)
	{
		put_comand_error(comand, "No such file or directory");
		return (-127);
	}
	if (stat(comand, &st) != 0)
		return (-127);

	if (S_ISDIR(st.st_mode))
	{
		put_comand_error(comand, "Is a directory");
		return (-126);
	}
	if (access(comand, X_OK) != 0)
	{
		put_comand_error(comand, "Permission denied");
		return (-126);
	}
	return (0);
}

int get_cmd_origin(char **args, t_origin *origin, t_all *all, int *redir_status)
{
	int	validation;

	if (!origin || !args || !all || !all->my_env || !redir_status)
		return (FAIL);
	if (!args[0])
		return (all->children_pids[all->node_nbr] = 0, *redir_status = false, 0);
	origin->abs_path = NULL;
	origin->builtin = NULL;
	origin->builtin = get_built_in(args[0]);
	if (origin->builtin == NULL)
	{
		if (args[0] && ((*args[0] == '.') || (*args[0] == '/')))
		{
			validation = validate_absolute_path(args[0]);
			if (validation < 0)
			{
				all->children_pids[all->node_nbr] = validation;
				*redir_status = false;
				return (0);			
			}
		}
		origin->abs_path = get_absolute_path("PATH", args[0], all->my_env->envp, all->buffer);
	}
	update_underline_on_env(origin->abs_path, all->my_env, args); // decide if goes to final version
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
	if (!!execve(abs_path, args, all->my_env->envp))
		perror("execve");
	end_structures(all, true, true, 127);
	return (1);
}

int exec_command(t_cmd *node, t_origin *origin, t_all *all)
{
	int pid;
	int node_nbr;

	if (!node || !origin || !all || !all->fds || !node->args)
		return (FAIL);
	node_nbr = all->node_nbr;
	pid = exec_fork(node, node_nbr, origin);
	all->children_pids[node_nbr] = pid;
	if (pid > 0)
		return (0);
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

	if (!all || !node || !fds || !env)
		return (0);
	all->node_nbr = 0;
	while (node != NULL)
	{
		get_pipe(fds, node);
		exec_redirections(all, node, &redir_status);
		exec_pipe(fds->pipe);
		get_cmd_origin(node->args, &origin, all, &redir_status);
		if (redir_status == true)
			exec_command(node, &origin, all);
		restore_fds(fds, node);
		all->node_nbr++;
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
