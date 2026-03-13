/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built-ins.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olacerda <olacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 22:17:42 by otlacerd          #+#    #+#             */
/*   Updated: 2026/03/10 22:39:56 by olacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILT_INS_H
# define BUILT_INS_H

# include "minishell.h"
# include "utils.h"
# include "core_execution.h"

//-built-ins_manager.c ---------------------------------------------------------
func_ptr	*get_built_in(char *comand);
int			exec_builtin(t_origin *origin, t_cmd *node, t_all *all);

//-env.c -----------------------------------------------------------------------
int			built_env(t_all *all, t_cmd *node, t_env *env, char *buffer);
int			env_show(char **envp, int is_export);
char		**create_env(char **envp, int *capacity);
int			assign_env_struct(t_env *env, char **envp, char *buffer);
int			assign_minimal_env(t_env *env, char *buffer);

// int		env_free(char **buffer, int size);
int			env_add(t_env *env_st, int line, char *key, char *string);
int			env_remove(t_env *env_st, char *key);
int			env_update(t_env *env_st, char *key, char *value1, char *value2);
int			env_find_line(char *key, char **envp);
char		*env_find_pointer(char *prefix, char **envp);

//-cd.c-------------------------------------------------------------------------
int			built_cd(t_all *all, t_cmd *node, t_env *env, char *buffer);
int			cd_minus(int oldpwd_status, char **new_path, char **envp);
int			change_paths(char *new_path, int *old_stat, t_env *env, char *buf);

//-echo.c-----------------------------------------------------------------------
int			built_echo(t_all *all, t_cmd *node, t_env *env, char *buffer);
int			parse_echo(char **args, int *line);

//-export.c --------------------------------------------------------------------
int			built_export(t_all *all, t_cmd *node, t_env *env, char *buffer);
int         export_with_arguments(t_cmd *node, int *line, t_env *env);
int			export_case(t_env *env, char *string);
char		**duplicate_envp(t_env *env_st);
int			sort_env(char **env);

//-export_utils.c --------------------------------------------------------------
char		*env_key_dup(char *string, int	delimiter);
char		*env_value_dup(char *string, int beginning);
char		*env_get_value(char *prefix, char **env);
int			parse_export_string(char *string);

//-unset.c ---------------------------------------------------------------------
int			built_unset(t_all *all, t_cmd *node, t_env *env, char *buffer);

//-exit.c ----------------------------------------------------------------------
int			built_exit(t_all *all, t_cmd *node, t_env *env, char *buffer);

//-pwc.c -----------------------------------------------------------------------
int			built_pwd(t_all *all, t_cmd *node, t_env *env, char *buffer);

#endif