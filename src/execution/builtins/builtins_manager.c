/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_manager.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otlacerd <otlacerd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2026/03/25 06:53:48 by otlacerd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <built_ins.h>

func_ptr * get_built_in(char *comand)
{
	if (!comand)
		return (NULL);
	if (string_compare(comand, "exit") == 0)
		return (built_exit);
	else if (string_compare(comand, "env") == 0)
		return (built_env);
	else if (string_compare(comand, "export") == 0)
		return (built_export);
	else if (string_compare(comand, "unset") == 0)
		return (built_unset);
	else if (string_compare(comand, "echo") == 0)
		return (built_echo);
	else if (string_compare(comand, "cd") == 0)
		return (built_cd);
	else if (string_compare(comand, "pwd") == 0)
		return (built_pwd);
	return (NULL);
}

int update_underline_on_env(char *abs_path, t_env *env, char **args)
{
	int line;

	if (!env || !args)
		return (FAIL);
	line = 0;
	if (abs_path != NULL)
		env_update(env, "_", "=", abs_path);
	else
	{
		line = 0;
		while (args[line] != NULL)
			line++;
		if (((line - 1) >= 0) && (args[line - 1] != NULL))
			env_update(env, "_", "=", args[line - 1]);
	}
	return (1);
}

int exec_builtin(t_origin *origin, t_cmd *node, t_all *all)
{
	if (!origin || !node || !all || !all->my_env)
		return (0);
	return (origin->builtin(all, node, all->my_env, all->buffer));
}
int	check_export_size(char *string, int *index)
{
	if (!string)
		return (0);	
	while (string[(*index)] && (string[(*index)] != '=')
		&& (string[(*index)] != '+'))
	{
		(*index)++;
	}
	if (!string[(*index)])
		return (0);
	return (1);
}

char	*get_export_new_value(char *string, int *index, char *old_value)
{
	if (!string)
		return (NULL);
	if (string[(*index)] && ((string[(*index)] == '=')
		|| ((string[(*index)] && (string[(*index)++] == '+'))
			&& (string[(*index)] == '='))))
	{
		if (old_value && ((*index) > 0) && (string[(*index) - 1] == '+'))
			(*index)++;
		return (env_value_dup_beginning(string, (*index)));
	}
	return (NULL);
}