/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nismayil <nismayil@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 02:20:46 by otlacerd          #+#    #+#             */
/*   Updated: 2026/03/25 03:43:47 by nismayil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <built_ins.h>
#include <core_execution.h>
#include <data.h>
#include <parser.h>
#include <utils.h>

int	main(int argc, char *argv[], char **envp)
{
	t_all		*all;
	t_parse_stat	res;

	all = init_structures();
	if (!all)
		return (1);
	fill_structures(all, argc, argv, envp);
	signals(false, 0);
	while (1)
	{
		fill_structs_on_loop(all);
		if (get_line(&(all->main_line), all) == false)
			break ;
		res = parse(all);
		if (res == PARSE_FAIL)
			break ;
		exec_all_heredocs(all);
		exec_all_comands(all, all->head, all->my_env->envp);
		end_structures(all, 0, 0, 0);
		free(all->main_line);
	}
	end_structures(all, 1, 0, 0);
	return (0);
}
