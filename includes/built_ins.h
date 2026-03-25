/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otlacerd <otlacerd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 22:17:42 by otlacerd          #+#    #+#             */
/*   Updated: 2026/03/25 06:35:33 by otlacerd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILT_INS_H
# define BUILT_INS_H

# include "core_execution.h"
# include "minishell.h"
# include "utils.h"

//-built-ins_manager.c ---------------------------------------------------------
func_ptr	*get_built_in(char *comand);
int			exec_builtin(t_origin *origin, t_cmd *node, t_all *all);
int			update_underline_on_env(char *absolute_path, t_env *env,
				char **args);

//-env.c -----------------------------------------------------------------------
int			built_env(t_all *all, t_cmd *node, t_env *env, char *buffer);
void		env_show(char **envp, int is_export);
char		**create_env(char **envp, int *capacity);
int			assign_env_struct(t_env *env, char **envp, char *buffer);
int			assign_minimal_env(t_env *env, char *buffer);

//-env_utils.c
int			env_add(t_env *env_st, int line, char *key, char *string);
int			env_remove(t_env *env_st, char *key);
int			env_update(t_env *env_st, char *key, char *value1, char *value2);
int			env_find_line(char *key, char **envp);
char		*env_find_pointer(char *prefix, char **envp);

//-cd.c-------------------------------------------------------------------------
int			built_cd(t_all *all, t_cmd *node, t_env *env, char *buffer);
char		*get_new_path(t_cmd *node, char **envp, int *cd_status);
int			change_paths(char *new_path, t_env *env, char *buf, int cd_status);

//-echo.c-----------------------------------------------------------------------
int			built_echo(t_all *all, t_cmd *node, t_env *env, char *buffer);
int			parse_echo(char **args, int *line);

//-export.c --------------------------------------------------------------------
int			built_export(t_all *all, t_cmd *node, t_env *env, char *buffer);
int			export_with_arguments(t_cmd *node, int *line, t_env *env);
int			export_case(t_env *env, char *string);
char		**duplicate_envp(t_env *env_st);
int			sort_env(char **env);

//-export_utils.c --------------------------------------------------------------
char		*env_key_dup(char *string, int delimiter);
// Careful,have Malloc
char		*env_get_value(char *prefix, char **env);
// Original,not malloc
char		*env_value_dup(char *prefix, char **env);
// Careful,have MALLOC
char		*env_value_dup_beginning(char *string, int beginning);
// have MALLOC
int			parse_export_string(char *string);

//-unset.c ---------------------------------------------------------------------
int			built_unset(t_all *all, t_cmd *node, t_env *env, char *buffer);

//-exit.c ----------------------------------------------------------------------
int			built_exit(t_all *all, t_cmd *node, t_env *env, char *buffer);
int			parse_exit(char **args);

//-exit_utils.c ----------------------------------------------------------------
int			is_overflow_long(char *string);
int			have_space_between(char *string);
int			string_trim(char **string, char *ref, char *set_remove);
int			is_normal_number(char *number);
int			have_space_between(char *string);

//-pwc.c -----------------------------------------------------------------------
int			built_pwd(t_all *all, t_cmd *node, t_env *env, char *buffer);

#endif
