/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nismayil <nismayil@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 02:41:35 by otlacerd          #+#    #+#             */
/*   Updated: 2026/03/13 18:44:53 by nismayil         ###   ########.fr       */
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

//-create_comand_lst.c----------------------------------------------------------
char **create_args_buffer(int line, char **all_args);
void assign_args(int *line, char **splitted, char **comand, char **args);
int create_node_comand(t_cmd **head, t_cmd **node, char *cmd, char **args);
int create_linked_list(char **all_args, t_cmd **head, t_all *all);

//-create_redir_lst.c ----------------------------------------------------------
int create_lst_redir(t_cmd *node, char **splitted, int line, int *heredoc_count);
int create_n_assign_node_redir(t_redir **head, char **splitted, int line);
int is_redirection(char *string);

//-list_comand_tools.c ---------------------------------------------------------
void comand_lstadd_back(t_cmd **lst, t_cmd *new);
void comand_lstclear(t_cmd **lst, void (*del)(void *));
int comand_lstsize(t_cmd *lst);
t_cmd *comand_lstnew(char *comand, char **args);

//-list_redir_tools.c ----------------------------------------------------------
void redir_lstadd_back(t_redir **lst, t_redir *new);
void redir_lstclear(t_redir **lst, void (*del)(void *));
int redir_lstsize(t_redir *lst);
t_redir *redir_lstnew(t_redir_type type, char *args);

#endif