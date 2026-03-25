/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otlacerd <otlacerd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 15:38:20 by olacerda          #+#    #+#             */
/*   Updated: 2026/03/25 06:05:02 by otlacerd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <built_ins.h>

int env_update(t_env *env_st, char *key, char *value1, char *value2)
{
	char *result;
	int line;
	int total_size;

	if (!key || !env_st->envp)
		return (0);
	result = NULL;
	line = env_find_line(key, env_st->envp);
	if (line == -1)
		line = env_st->size;
	total_size = 0;
	total_size += string_length(value1);
	total_size += string_length(value2);
	if (total_size > 0)
	{
		result = malloc((total_size + 1) * sizeof(char));
		if (!result)
			return (0);
	}
	string_zero(result, (total_size + 1));
	string_cat(result, (total_size + 1), value1);
	string_cat(result, (total_size + 1), value2);
	env_add(env_st, line, key, result);
	free(result);
	return (1);
}

char *env_find_pointer(char *prefix, char **envp)
{
	int result;
	int line;

	line = 0;
	if (!prefix || !envp)
		return (NULL);
	while (envp[line] != NULL)
	{
		result = compare_prefix(prefix, envp[line]);
		if (result > 0)
			return (envp[line]);
		line++;
	}
	return (NULL);
}

int env_find_line(char *key, char **envp)
{
	int line;

	if (!key || !envp)
		return (-1);
	line = 0;
	while (envp[line] != NULL)
	{
		if (compare_prefix(key, envp[line]) == true)
			return (line);
		line++;
	}
	return (-1);
}

int env_add(t_env *env_st, int line, char *key, char *string)
{
	int size;

	if (!env_st || !env_st->envp || (line < 0))
		return (0);
	if (line >= env_st->capacity)
	{
		env_st->envp = re_allocker(env_st->envp, env_st->size + 1,
			env_st->size + ENV_INCREMENT + 1, sizeof(char *));
		if (!env_st->envp)
			return (0);
		env_st->capacity = env_st->size + ENV_INCREMENT;
	}
	size = (string_length(key) + string_length(string));
	free(env_st->envp[line]);
	env_st->envp[line] = malloc((size + 1) * sizeof(char));
	if (!env_st->envp[line])
		return (0);
	string_copy(env_st->envp[line], key);
	string_cat(env_st->envp[line], (size + 1), string);
	if (line >= env_st->size)
	{
		env_st->envp[++line] = NULL;
		env_st->size = line;
	}
	return (1);
}

int env_remove(t_env *env_st, char *key)
{
	int line;

	if (!env_st || !key)
		return (0);
	line = -1;
	while (env_st->envp[++line] != NULL)
	{
		if (compare_prefix(key, env_st->envp[line]) == true)
		{
			free(env_st->envp[line]);
			line--;
			while (++line < env_st->size)
				env_st->envp[line] = env_st->envp[line + 1];
			env_st->size--;
			break;
		}
	}
	if ((env_st->capacity - env_st->size) >= (2 * ENV_INCREMENT))
	{
		env_st->envp = re_allocker(env_st->envp, env_st->size + 1,
								   (env_st->capacity - ENV_INCREMENT) + 1, sizeof(char *));
		env_st->capacity = env_st->capacity - ENV_INCREMENT;
	}
	return (0);
}
