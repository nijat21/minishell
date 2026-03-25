/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otlacerd <otlacerd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 02:41:35 by otlacerd          #+#    #+#             */
/*   Updated: 2026/03/25 06:02:39 by otlacerd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DATA_H
# define DATA_H

# include "core_execution.h"
# include "minishell.h"
# include "signals.h"
# include "utils.h"

//-data.c-----------------------------------------------------------------------
t_all	*init_structures(void);
int		fill_structures(t_all *all, int argc, char **argv, char **envp);
int		fill_structs_on_loop(t_all *all);
int		get_line(char **line, t_all *all);
void	end_structures(t_all *all, int is_the_end, int is_child, int status);

//-data_utils.c ----------------------------------------------------------------
int	comand_lstsize(t_cmd *lst);

#endif
