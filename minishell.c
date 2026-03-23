/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nismayil <nismayil@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 02:20:46 by otlacerd          #+#    #+#             */
/*   Updated: 2026/03/23 14:39:37 by nismayil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <core_execution.h>
#include <data.h>
#include <utils.h>
#include <built-ins.h>
#include <parser.h>

int g_signal = 0;

int main(int argc, char *argv[], char **envp)
{
	t_all *all;
	t_parse_stat res;

	all = init_structures();
	if (!all)
		return (1);
	fill_structures(all, argc, argv, envp);
	signals(false, 0);
	while (1)
	{
		fill_structs_on_loop(all);
		if (get_line(&(all->main_line), all) == false)
			break;
		res = parse(all);
		if (res == PARSE_FAIL)
			break;
		// print_command(all->head);
		exec_all_heredocs(all);
		exec_all_comands(all, all->head, all->my_env->envp);
		end_structures(all, 0, 0, 0);
		free(all->main_line);
	}
	end_structures(all, 1, 0, 0);
	return (0);
}
