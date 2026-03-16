/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nismayil <nismayil@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 02:41:35 by otlacerd          #+#    #+#             */
/*   Updated: 2026/03/14 23:38:52 by nismayil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DATA_H
#define DATA_H

#include "minishell.h"
#include "core_execution.h"
#include "signals.h"
#include "utils.h"

//-data.c-----------------------------------------------------------------------
t_all *init_structures(void);
int fill_structures(t_all *all, int argc, char **argv, char **envp);
int fill_structs_on_loop(t_all *all);
void end_structures(t_all *all, int is_the_end, int is_children);
int get_line(char **line);

//-data_utils.c ----------------------------------------------------------
int is_redirection(char *string);

#endif