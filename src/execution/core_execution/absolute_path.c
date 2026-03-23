/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   absolute_path.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otlacerd <otlacerd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 02:53:45 by otlacerd          #+#    #+#             */
/*   Updated: 2026/03/23 12:45:15 by otlacerd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <core_execution.h>

int	parse_path(char *path)
{
	int	index;

	if (!path)
		return (0);
	index = 0;
	if (path[index] == '\0')
		return (false);
	if (path[index] == '.')
	{
		index++;
		if (path[index] == '\0')
			return (false);
		if (path[index] == '.')
		{
			index++;
			if (path[index] == '\0')
				return (false);
		}
	}
	return (true);
}

int get_next_path(char *path, char *environment_variable, int env_idx, int path_buffer_size)
{
	int index;

	index = 0;
	if (!path || !environment_variable || !env_idx)
		return (0);
	while ((environment_variable[env_idx]) && (index < (path_buffer_size - 1)) && (environment_variable[env_idx] != ':'))
	{
		path[index] = environment_variable[env_idx];
		index++;
		env_idx++;
	}
	path[index] = '\0';
	return (index);
}

int append_comand(char *path, char *comand, int path_idx, int path_buffer_size)
{
	int index;

	if (!path || !comand || !path_buffer_size || !path_idx)
		return (0);
	index = 0;
	if (path_idx < (path_buffer_size - 1))
	{
		path[path_idx] = '/';
		path_idx++;
	}
	while ((path_idx < (path_buffer_size - 1)) && (comand[index] != '\0'))
	{
		path[path_idx] = comand[index];
		path_idx++;
		index++;
	}
	path[path_idx] = '\0';
	return (1);
}

char *find_abs_path(char *env_path, char *comand, int prefix_size, char *buffer)
{
	int env_path_size;
	int path_idx;
	int path_size;

	if (!env_path || !comand || !prefix_size)
		return (NULL);
	string_zero(buffer, PATH_MAX); 
	env_path_size = string_length(env_path);
	path_idx = prefix_size;
	path_size = 0;
	if (!env_path_size || env_path_size <= prefix_size)
		return (NULL);
	while ((path_idx < env_path_size) && env_path[path_idx])
	{
		path_size = get_next_path(buffer, env_path, path_idx, PATH_MAX);
		if (append_comand(buffer, comand, path_size, PATH_MAX) == false)
			return (NULL);
		if (is_accessible(buffer) == true)
			return (buffer);
		path_idx += path_size;
		if (env_path[path_idx] == ':')
			path_idx++;
	}
	return (NULL);
}

char *get_absolute_path(char *prefix, char *comand, char **envp, char *buffer)
{
	char *env_path;
	char *absolute_path;
	int prefix_size;

	if (!prefix || !comand || !envp)
		return (NULL);
	if (is_redirection(comand) || ((comand[0] == '.') && !comand[1]))
		return (NULL);
	if (!(*comand) || ((comand[0] == '.') && (comand[1] == '.') && !comand[2]))
		return (NULL);
	if (is_accessible(comand) == true)
		return (comand);
	env_path = NULL;
	absolute_path = NULL;
	prefix_size = string_length(prefix);
	if (!prefix_size)
		return (NULL);
	env_path = env_find_pointer(prefix, envp);
	if (!env_path)
		return (NULL);
	absolute_path = find_abs_path(env_path, comand, prefix_size, buffer);
	return (absolute_path);
}
