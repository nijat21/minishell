/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core_execution.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otlacerd <otlacerd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 02:54:14 by otlacerd          #+#    #+#             */
/*   Updated: 2026/03/22 22:20:37 by otlacerd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CORE_EXECUTION_H
#define CORE_EXECUTION_H

#include "minishell.h"
#include "utils.h"
#include "data.h"
#include "built-ins.h"

//-absolute_path.c -------------------------------------------------------------
char *get_absolute_path(char *prefix, char *comand, char **envp, char *buffer);
char *find_abs_path(char *env_path, char *comand, int prefix_size, char *buffer);
int append_comand(char *path, char *comand, int path_idx, int path_size);
int get_next_path(char *path, char *env_var, int env_idx, int path_size);
int	parse_path(char *path);

//-core_execution.c ------------------------------------------------------------
int	exec_all_comands(t_all *all, t_cmd *node, char **envp);
int exec_linked_lst(t_all *all, t_cmd *node, t_fds *fds, t_env *env);
int get_cmd_origin(char **arg, t_env *envp, t_origin *origin, char *buffer);
int	exec_command(t_cmd *node, t_origin *origin, t_all *all);
int	exec_external_cmd(char *abs_path, char **args, t_all *all);

//-core_execution_utils.c ------------------------------------------------------
int compare_prefix(char *prefix, char *string);
int is_builtin(t_origin *origin);
int	is_external_comand(t_origin *origin);
int	has_next_comand(t_cmd *node, t_origin *origin);
int is_accessible(char *path);

//-pipe.c ---------------------------------------------------------------------
int exec_pipe(int *fds);
int get_pipe(t_fds *fds, t_cmd *node);

//-pid_n_exit_status.c ---------------------------------------------------------
int	exec_fork(t_cmd *node, int node_nbr, t_origin *origin);
int wait_all_children(int *children_pids, int size, int *exit_status, int out_backup);
int create_children_pids_buffer(int **children_pids, int size);
int update_exit_status(int *exit_status, int status, int out_backup, int is_child);
void handle_exit_status(void);


//-fd.c ------------------------------------------------------------------------
int save_original_fds(int *std_backup);
int	close_pipe_fds(int *pipe_fds);
int destroy_fds(t_fds *fds, int is_the_end);
int restore_fds(t_fds *fds, t_cmd *node);
int safe_close_fd(int *fd);

//-redir_execution.c -----------------------------------------------------------
int	exec_redirections(t_all *all, t_cmd *node, t_fds *fds, int *redir_status);
int sync_redir_n_pipe(t_cmd *node, t_redir *redir, int *red_fd, int *pp_fd);

//-redirections.c --------------------------------------------------------------
int redir_in(t_redir *redir, int fds[2]);
int redir_out(t_redir *redir, int fds[2]);
int redir_append(t_redir *redir, int fds[2]);
int redir_heredoc(t_redir *redir, int fds[2], char **temps, int count);
int is_redirection(char *string);

//-heredoc_execution.c
int exec_all_heredocs(t_all *all);
int exec_heredoc(t_all *all, t_redir *redir, char **temps, int index);
int exec_heredoc_content(t_all *all, int *signal, t_redir *redir, int fd);
int read_write_content(t_all *all, t_redir *redir, int stdin_backup, int fd);

//-heredoc_utils.c
char 	**create_heredoc_temps_buffer(int size);
int 	add_heredoc_history(char *buffer, char *user_line, int size, char *path);
char 	*create_heredoc_temp_name(int index, char *std_name);
int		count_heredocs(t_cmd *head);
void 	unlink_all_heredoc_temps(char **heredoc_temps);


#endif
