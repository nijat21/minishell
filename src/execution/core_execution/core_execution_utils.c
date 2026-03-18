/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core_execution_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otlacerd <otlacerd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 02:53:08 by otlacerd          #+#    #+#             */
/*   Updated: 2026/03/17 19:26:35 by otlacerd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <core_execution.h>

int	is_accessible(char *path, char *comand)
{
	int	result;

	if (!path || !comand)
		return (0);
	result = 0;
	result = access((const char *)path, X_OK);
	if (result == 0)
		return (1);
	return (0);
}

int	compare_prefix(char *prefix, char *string)
{
	int	index;

	index = 0;
	if (!prefix || !string)
		return (0);
	while ((prefix[index]) && (string[index]) && prefix[index] == string[index])
	{
		index++;
	}
	if (prefix[index] == '\0')
	{
		return (1);
	}
	return (0);
}

int	has_next_comand(t_cmd *node)
{
	if (!node)
		return (FAIL);
	if (node->next == NULL)
		return (false);
	return (true);
}

int	is_external_comand(t_origin *origin)
{
	if (!origin)
		return (FAIL);
	if (origin->abs_path != NULL)
		return (true);
	return (false);
}

int	update_underline_on_env(char *absolute_path, t_env *env, char **args)
{
	int	line;

	if (!env || !args)
		return (FAIL);
	line = 0;
	if (absolute_path != NULL)
		env_update(env, "_", "=", absolute_path);
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
int is_builtin(t_origin *origin)
{
	if (!origin)
		return (FAIL);
	if (origin->builtin != NULL)
		return (true);
	return (false);
}