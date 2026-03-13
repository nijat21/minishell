/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nismayil <nismayil@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 06:58:43 by olacerda          #+#    #+#             */
/*   Updated: 2026/03/13 19:04:28 by nismayil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H
#define SIGNALS_H

#include "minishell.h"

void handler(int sig);
t_proc *get_process_info(t_all *all);
void signals(int is_child);

#endif