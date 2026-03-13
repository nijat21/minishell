/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core_execution.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olacerda <olacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 04:34:05 by olacerda          #+#    #+#             */
/*   Updated: 2026/03/11 14:56:11 by olacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <core_execution.h>

int	get_cmd_origin(char **args, t_env *env, t_origin *origin, char *buffer)
{
	func_ptr *builtin;
	char *absolute_path;
	int	line;
	
	absolute_path = NULL;
	builtin = NULL;
	if (!origin || !args)
		return (0);
	builtin = get_built_in(args[0]);
	if (builtin == NULL)
		absolute_path = get_absolute_path("PATH", args[0], env->envp, buffer);
	if (absolute_path != NULL)
		env_update(env, "_", "=", absolute_path);
	else
	{
		line = 0;
		while (args[line] != NULL)
			line++;
		if (((line - 1) >= 0) && (args[line - 1] != NULL))
			env_update(env, "_", "=", args[line - 1]);
	}
	origin->abs_path = absolute_path;
	origin->built_in = builtin;
	return (1);
}

int	exec_external_cmd(char *abs_path, char **args, t_all *all, int *fds)
{
	int	pid;

	if (!args || !all || !all->my_env || !all->my_env->envp || !fds)
		return (0);
	pid = fork();
	if (pid < 0)
		return (perror("fork"), 0);
	if (pid == 0)
	{
		signals(true);
		tcsetattr(STDIN_FILENO, TCSANOW, &(all->saved_termios));
		rl_clear_history();
		if (abs_path == NULL)
			return (put_comand_error(args[0], "comand not found"), exit(127), 0);
		else if (!!execve(abs_path, args, all->my_env->envp))
		{
			restore_original_fds(all->fds);
			perror("execve");
		}
		handle_exit_status();
		end_structures(all, 1, 1);
	}
	else if (pid > 0)
	{
		if (fds[0] >= 0)
			safe_close_fd(&fds[0]);
		if (fds[1] >= 0)
			safe_close_fd(&fds[1]);
	}
	return (pid);
}

int	exec_linked_lst(t_all *all, t_cmd *node, t_fds *fds, t_env *env)
{
	t_origin origin;
	int		status;
	int		*node_nbr;

	if (!all || !node || !fds || !env)
		return (0);
	node_nbr = &(all->node_number);
	while (node != NULL)
	{
		restore_original_fds(fds);
		get_pipe(fds, node);
		status = exec_redirections(all, node, fds->redir, fds->pipe);
		exec_pipe(fds->pipe);
		get_cmd_origin(node->args, env, &origin, all->buffer);
		if ((status == true) && (origin.built_in != NULL))
			exec_builtin(&origin, node, all);
		else if (status == true)
			all->children_pids[(*node_nbr)] = exec_external_cmd(origin.abs_path, node->args, all, fds->pipe);
		else
			all->children_pids[*node_nbr] = -1;
		all->node_number++;
		node = node->next;
 	}
	return (1);
}

int	exec_comands(t_all *all, t_cmd *node, char **envp)
{
	if (!all || !node || !envp || (all->process_info->signal == SIGINT))
		return (0);
	create_children_pids_buffer(&all->children_pids, all->lst_size);
	exec_linked_lst(all, node, all->fds, all->my_env);
	wait_all_children(all->children_pids, all->node_number, &(all->process_info->exit_status));
	return (1);
}
