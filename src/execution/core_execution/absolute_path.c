/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   absolute_path.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otlacerd <otlacerd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 02:53:45 by otlacerd          #+#    #+#             */
/*   Updated: 2026/03/18 06:22:19 by otlacerd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <core_execution.h>

int	get_next_path(char *path, char *environment_variable, int env_idx, int path_buffer_size)
{
	int	index;

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

int	append_comand(char *path, char *comand, int path_idx, int path_buffer_size)
{
	int	index;

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

char *find_abs_path(char *environment_variable, char *comand, int prefix_size, char *path)
{
	int			env_size;
	int			env_idx;
	int			path_size;
	
	if (!string_zero(path, PATH_MAX) || !environment_variable || !comand || !prefix_size)
		return (NULL);
	env_size = string_length(environment_variable);
	env_idx = prefix_size;
	path_size = 0;
	if (!env_size || env_size <= prefix_size)
		return (NULL);
	while ((env_idx < env_size) && environment_variable[env_idx])
	{
		path_size = get_next_path(path, environment_variable, env_idx, PATH_MAX);
		if (append_comand(path, comand, path_size, PATH_MAX) == false)
			return (NULL);
		if (is_accessible(path) == true)
		{
			dprintf(2, "%s\n", path);
			return (path);
		}
		env_idx += path_size;
		if (environment_variable[env_idx] == ':')
			env_idx++;
	}
	return (NULL);
}

char *get_absolute_path(char *prefix, char *comand, char **envp, char *buffer)
{
	char *environment_variable;
	char *absolute_path;
	int	prefix_size;

	if (!prefix || !comand || !envp)
		return (NULL);
	if (is_accessible(comand) == true)
		return (comand);
	if (is_redirection(comand))
		return (NULL);
	environment_variable = NULL;
	absolute_path = NULL;
	prefix_size = string_length(prefix);
	if (!prefix_size)
		return (NULL);
	environment_variable = env_find_pointer(prefix, envp);
	if (!environment_variable)
		return (NULL);
	absolute_path = find_abs_path(environment_variable, comand, prefix_size, buffer);
	return (absolute_path);
}
