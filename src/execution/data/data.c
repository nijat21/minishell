/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otlacerd <otlacerd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 02:43:11 by otlacerd          #+#    #+#             */
/*   Updated: 2026/03/19 07:16:52 by otlacerd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <data.h>
#include <parser.h>
#include <built-ins.h>
#include <core_execution.h>

t_all *init_structures(void)
{
	t_all *all;

	all = malloc(sizeof(t_all));
	if (!all)
		return (end_structures(all, 1, 0, 1), NULL);
	*all = (t_all){0};
	all->fds = malloc(sizeof(t_fds));
	if (!all->fds)
		return (end_structures(all, 1, 0, 1), NULL);
	*all->fds = (t_fds){0};
	all->my_env = malloc(sizeof(t_env));
	if (!all->my_env)
		return (end_structures(all, 1, 0, 1), NULL);
	*(all->my_env) = (t_env){0};
	all->process_info = malloc(sizeof(t_proc));
	if (!all->process_info)
		return (end_structures(all, 1, 0, 1), NULL);
	*(all->process_info) = (t_proc){0};
	return (all);
}

int	fill_structs_on_loop(t_all *all)
{
	if (!all)
		return (0);
	if (!restore_original_fds(all->fds))
		return (end_structures(all, 1, 0, 1), 0);
	string_zero(all->buffer, PATH_MAX);
	all->fds->previous_0 = -1;
	all->heredoc_temps = NULL;
	all->main_line = NULL;
	all->head = NULL;
	all->children_pids = NULL;
	all->lst_size = 0;
	all->process_info->signal = 0;
	all->heredoc_count = 0;
	errno = 0;
	return (1);
}

int	fill_structures(t_all *all, int argc, char **argv, char **envp)
{
	if (!all || !argc || !argv)
		return (0);
	all->father_pid = getpid();
	tcgetattr(STDIN_FILENO, &(all->saved_termios));
	if (!save_original_fds(all->fds->std_backup))
		return (end_structures(all, 1, 0, 1), exit(1), 0);
	if (!assign_env_struct(all->my_env, envp, all->buffer))
		return (end_structures(all, 1, 0, 1), exit(1), 0);
	all->argv = argv;
	all->envp = envp;
	all->argc = argc;
	get_process_info(all);
	return (1);
}

void	end_structures(t_all *all, int is_the_end, int is_child, int status)
{
	if (!all)
		return ;
	if (is_child == false)
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
		destroy_fds(all->fds, 1);
		if (all->fds)
			free(all->fds);
		if ((all->my_env))
			free_array_string(all->my_env->envp, 0);
		free(all->my_env);
		if (all->process_info)
			free(all->process_info);
		free(all);
		rl_clear_history();
		exit (status);
	}
}

int	get_line(char **line, t_all *all)
{
	(*line) = readline("minishell> ");
	if (!(*line))
	{
		write(2, "exit\n", 5);	
		return (0);
	}
	else if (*line)
	{
		all->main_line_count++;
		if (*(*line))
		{
			add_history((*line));
			all->process_info->signal = 0;			
		}
	}
	return (1);
}


