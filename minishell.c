/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otlacerd <otlacerd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 02:20:46 by otlacerd          #+#    #+#             */
/*   Updated: 2026/03/17 00:52:32 by otlacerd         ###   ########.fr       */
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
	t_cmd *cmd;

	all = init_structures();
	if (!all)
		return (1);
	fill_structures(all, argc, argv, envp);
	// set_signal(S_PARENT); // nijat version
	signals(false);
	while (1)
	{
		fill_structs_on_loop(all);
		if (get_line(&(all->main_line)) == false)
		break;
		cmd = parse_tokens(all->main_line, &(all->process_info->exit_status));
		if (!cmd)
		{
			command_lstclear(&cmd);
			break;
		}
		all->head = cmd;
		exec_all_heredocs(all);
		exec_all_comands(all, all->head, all->my_env->envp);
		end_structures(all, 0, 0, 0);
		free(all->main_line);
	}
	end_structures(all, 1, 0, 0);
	return (0);
}
