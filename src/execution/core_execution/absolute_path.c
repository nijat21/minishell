/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   absolute_path.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otlacerd <otlacerd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 02:53:45 by otlacerd          #+#    #+#             */
/*   Updated: 2026/03/25 06:29:39 by otlacerd         ###   ########.fr       */
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

int	get_next_path(char *path, char *env_var, int env_idx, int path_buffer_size)
{
	int	index;
	int	index;

	index = 0;
	if (!path || !env_var || !env_idx)
		return (0);
	while ((env_var[env_idx]) && (index < (path_buffer_size - 1))
		&& (env_var[env_idx] != ':'))
	{
		path[index] = env_var[env_idx];
		index++;
		env_idx++;
	}
	path[index] = '\0';
	return (index);
}

int	append_comand(char *path, char *comand, int path_idx, int path_buffer_size)
int	append_comand(char *path, char *comand, int path_idx, int path_buffer_size)
{
	int	index;
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

char	*find_abs_path(char *env_path, char *comand, int pfx_size, char *buff)
{
	int	env_path_size;
	int	path_idx;
	int	path_size;
	int	env_path_size;
	int	path_idx;
	int	path_size;

	if (!env_path || !comand || !pfx_size)
		return (NULL);
	string_zero(buff, PATH_MAX);
	env_path_size = string_length(env_path);
	path_idx = pfx_size;
	path_size = 0;
	if (!env_path_size || env_path_size <= pfx_size)
		return (NULL);
	while ((path_idx < env_path_size) && env_path[path_idx])
	{
		path_size = get_next_path(buff, env_path, path_idx, PATH_MAX);
		if (append_comand(buff, comand, path_size, PATH_MAX) == false)
			return (NULL);
		if (is_accessible(buff) == true)
			return (buff);
		path_idx += path_size;
		if (env_path[path_idx] == ':')
			path_idx++;
	}
	return (NULL);
}

char	*get_absolute_path(char *prefix, char *cmd, char **envp, char *buffer)
{
	char	*env_path;
	char	*absolute_path;
	int		prefix_size;
	char	*env_path;
	char	*absolute_path;
	int		prefix_size;

	if (!prefix || !cmd || !envp)
		return (NULL);
	if (is_redirection(cmd) || ((cmd[0] == '.') && !cmd[1]))
		return (NULL);
	if (!(*cmd) || ((cmd[0] == '.') && (cmd[1] == '.') && !cmd[2]))
		return (NULL);
	if (is_accessible(cmd) == true)
		return (cmd);
	env_path = NULL;
	absolute_path = NULL;
	prefix_size = string_length(prefix);
	if (!prefix_size)
		return (NULL);
	env_path = env_find_pointer(prefix, envp);
	if (!env_path)
		return (NULL);
	absolute_path = find_abs_path(env_path, cmd, prefix_size, buffer);
	return (absolute_path);
}
