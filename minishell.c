/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otlacerd <otlacerd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 02:20:46 by otlacerd          #+#    #+#             */
/*   Updated: 2026/03/24 20:30:31 by otlacerd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <built-ins.h>
#include <core_execution.h>
#include <data.h>
#include <parser.h>
#include <utils.h>

int	g_signal = 0;

int	main(int argc, char *argv[], char **envp)
{
	t_all			*all;
	t_parse_stat	res;

	all = init_structures();
	if (!all)
		return (1);
	fill_structures(all, argc, argv, envp);
	signals(false, 0);
	while (1)
	{
		fill_structs_on_loop(all);
		// dprintf(2, "exit code1: %d\n", all->process_info->exit_status);
		if (get_line(&(all->main_line), all) == false)
			break ;
		res = parse(all);
		if (res == PARSE_FAIL)
			break ;
		// dprintf(2, "exit code2: %d\n", all->process_info->exit_status);
		exec_all_heredocs(all);
		// dprintf(2, "exit code3: %d\n", all->process_info->exit_status);
		exec_all_comands(all, all->head, all->my_env->envp);
		end_structures(all, 0, 0, 0);
		free(all->main_line);
	}
	end_structures(all, 1, 0, 0);
	return (0);
}
