/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otlacerd <otlacerd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 00:59:03 by otlacerd          #+#    #+#             */
/*   Updated: 2026/03/25 06:07:32 by otlacerd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "built_ins.h"

char **duplicate_envp(t_env *env_st)
{
	char **result;
	int line;

	if (!env_st)
		return (NULL);
	result = malloc((env_st->size + 1) * sizeof(char *));
	if (!result)
		return (NULL);
	line = 0;
	while (env_st->envp[line])
	{
		result[line] = env_st->envp[line];
		line++;
	}
	result[line] = NULL;
	return (result);
}

int sort_env(char **env)
{
	int line;
	int smallest;
	int finder;

	if (!env)
		return (0);
	line = 0;
	smallest = 0;
	finder = 0;
	while (env[line])
	{
		smallest = line;
		finder = line + 1;
		while (env[finder])
		{
			if (string_compare(env[smallest], env[finder]) > 0)
				smallest = finder;
			finder++;
		}
		if (line != smallest)
			string_swap(&(env[line]), &(env[smallest]));
		line++;
	}
	return (1);
}

int export_case(t_env *env, char *string)
{
	char *key;
	char *new_value;
	char *old_value;
	int index;

	if (!string || !env || !env->envp)
		return (-1);
	index = 0;
	key = NULL;
	new_value = NULL;
	old_value = NULL;
	while (string[index] && (string[index] != '=') && (string[index] != '+'))
		index++;
	key = env_key_dup(string, index);
	if (!key)
		return (0);
	if (string[index] != '=')
	{
		old_value = env_value_dup_beginning(env_find_pointer(key, env->envp),
			index);		
	}
	new_value = get_export_new_value(string, &index, old_value);
	env_update(env, key, old_value, new_value);
	return (free(key), free(old_value), free(new_value), 1);
}

int export_with_arguments(t_cmd *node, int *line, t_env *env)
{
	int to_return;

	if (!node || !line || !env)
		return (-1);
	to_return = 0;
	while (node->args[(*line)])
	{
		if (parse_export_string(node->args[(*line)]) == true)
			export_case(env, node->args[(*line)]);
		else
		{
			put_error("export: `");
			put_error(node->args[(*line)]);
			put_error("': not a valid identifier\n");
			to_return = -1;
		}
		(*line)++;
	}
	return (to_return);
}

int built_export(t_all *all, t_cmd *node, t_env *env, char *buffer)
{
	char **temp;
	int line;

	if (!env || !node || !node->args)
		return (-1);
	(void)buffer;
	(void)all;
	line = 1;
	if (!node->args[line])
	{
		temp = duplicate_envp(env);
		if (!temp)
			return (-1);
		sort_env(temp);
		env_show(temp, 1);
		free(temp);
		return (0);
	}
	return (export_with_arguments(node, &line, env));
}
