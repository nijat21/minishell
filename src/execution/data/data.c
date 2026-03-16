/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nismayil <nismayil@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 02:43:11 by otlacerd          #+#    #+#             */
/*   Updated: 2026/03/14 22:56:55 by nismayil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <data.h>
#include <parser.h>
#include <built-ins.h>

t_all *init_structures(void)
{
	t_all *all;

	all = malloc(sizeof(t_all));
	if (!all)
		return (put_error("Error\nFailed to allocate struct 'ALL'\n"), end_structures(all, 1, 0), NULL);
	*all = (t_all){0};
	all->fds = malloc(sizeof(t_fds));
	if (!all->fds)
		return (put_error("Error\nFailed to allocate struct 'fds'\n"), end_structures(all, 1, 0), NULL);
	*all->fds = (t_fds){0};
	all->my_env = malloc(sizeof(t_env));
	if (!all->my_env)
		return (put_error("Error\nFailed to allocate all->my_env in init_structs\n"), end_structures(all, 1, 0), NULL);
	*(all->my_env) = (t_env){0};
	all->process_info = malloc(sizeof(t_proc));
	if (!all->process_info)
		return (put_error("Error\nFailed to allocate all->process_info\n"), end_structures(all, 1, 0), NULL);
	*(all->process_info) = (t_proc){0};
	return (all);
}

int	fill_structs_on_loop(t_all *all)
{
	if (!all)
		return (0);
	errno = 0;
	string_zero(all->buffer, PATH_MAX);
	all->father_pid = getpid();
	all->heredoc_temps = NULL;
	all->main_line = NULL;
	all->head = NULL;
	all->node_number = 0;
	all->lst_size = 0;
	all->children_pids = NULL;
	all->fds->pipe[0] = -1;
	all->fds->pipe[1] = -1;
	if (!restore_original_fds(all->fds))
		return (write(2, "Failed to restore_original_fds\n", 31),
			end_structures(all, 1, 0), exit (1), 0);
	all->fds->redir[0] = STDIN_FILENO;
	all->fds->redir[1] = STDOUT_FILENO;
	all->fds->previous_0 = -1;
	all->process_info->signal = 0;
	all->splitted = NULL;
	all->heredoc_count = 0;
	return (1);
}

int	fill_structures(t_all *all, int argc, char **argv, char **envp)
{
	if (!all || !argc || !argv)
		return (0);
	tcgetattr(STDIN_FILENO, &(all->saved_termios));
	get_process_info(all);
	all->process_info->exit_status = 0;
	string_zero(all->buffer, PATH_MAX);
	all->heredoc_count = 0;
	all->argv = argv;
	all->envp = envp;
	all->argc = argc;
	all->head = NULL;
	all->children_pids = NULL;
	if (!save_original_fds(all->fds->std_backup))
		return (end_structures(all, 1, 0), exit (1), 0);
	all->fds->redir[0] = STDIN_FILENO;
	all->fds->redir[1] = STDOUT_FILENO;
	all->fds->previous_0 = -1;
	if (!assign_env_struct(all->my_env, envp, all->buffer))
		return (end_structures(all, 1, 0), exit (1), 0);
	return (1);
}

void	end_structures(t_all *all, int is_the_end, int is_children)
{
	if (!all)
		return ;
	if (is_children == false)
		unlink_all_heredoc_temps(all->heredoc_temps);
	if (all->heredoc_temps)
		free_array_string(all->heredoc_temps, 0);
	destroy_fds(all->fds, 0);
	if (all->head != NULL)
		command_lstclear(&all->head);
	if (all->children_pids)
		free(all->children_pids);
	if (is_the_end == true)
	{
		rl_clear_history();
		destroy_fds(all->fds, 1);
		if (all->fds)
			free(all->fds);
		if ((all->my_env))
			free_array_string(all->my_env->envp, 0);
		free(all->my_env);
		if (all->process_info)
			free(all->process_info);
		free(all);
		exit (1);
	}
}

int	get_line(char **line)
{
	(*line) = readline("minishell> ");
	if (!(*line))
	{
		write(2, "exit\n", 5);	
		return (0);
	}
	else if ((*line) && *(*line))
		add_history((*line));
	return (1);
}


