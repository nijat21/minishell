/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otlacerd <otlacerd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 02:43:11 by otlacerd          #+#    #+#             */
/*   Updated: 2026/03/23 15:12:13 by otlacerd         ###   ########.fr       */
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
	if (!restore_fds(all->fds, NULL))
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
	all->node_nbr = 0;
	errno = 0;
	return (1);
}

int	fill_structures(t_all *all, int argc, char **argv, char **envp)
{
	if (!all || !argc || !argv)
		return (0);
	tcgetattr(STDIN_FILENO, &(all->saved_termios));
	if (!save_original_fds(all->fds->std_backup))
		return (end_structures(all, 1, 0, 1), exit(1), 0);
	if (!assign_env_struct(all->my_env, envp, all->buffer))
		return (end_structures(all, 1, 0, 1), exit(1), 0);
	all->argv = argv;
	all->envp = envp;
	all->argc = argc;
	all->fds->dev_null = open("/dev/null", O_WRONLY);
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
	// dprintf(2, "here\n");
	destroy_fds(all->fds, false);
	// dprintf(2, "here2\n");
	if (all->head != NULL)
		command_lstclear(&all->head);
	// dprintf(2, "here3\n");
	if (all->children_pids)
		free(all->children_pids);
	if (is_the_end == true)
	{
		destroy_fds(all->fds, true);
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
	static int interactive_mode = -1;

	if (!line || !all)
		return (0);
	if (interactive_mode == -1)
		interactive_mode = isatty(STDIN_FILENO);
	if (interactive_mode == false)
		dup2(all->fds->dev_null, STDOUT_FILENO);
	(*line) = readline("minishell> ");
	if (!(*line))
	{
		if (interactive_mode == true)
			write(STDERR_FILENO, "exit\n", 5);
		return (0);
	}
	if (*line)
	{
		if (interactive_mode == false)
			dup2(all->fds->std_backup[1], STDOUT_FILENO);
		all->main_line_count++;
		if (*(*line))
			add_history((*line));
		all->process_info->signal = 0;
	}
	return (1);
}


