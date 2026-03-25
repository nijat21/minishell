/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otlacerd <otlacerd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 03:15:12 by username          #+#    #+#             */
/*   Updated: 2026/03/25 06:04:12 by otlacerd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   cd.c                                               :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: olacerda <olacerda@student.42.fr>          +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2026/02/26 02:44:14 by olacerda          #+#    #+#             */
// /*   Updated: 2026/02/26 03:20:44 by olacerda         ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

#include <built_ins.h>

int	check_dot_edgecase(char **arg, t_env *env)
{
	int		size;
	char	*string;
	int		size;
	char	*string;

	if (!arg || !*arg)
		return (0);
	string = NULL;
	if ((*arg) && (((*arg)[0] == '.' && !(*arg)[1])
				|| ((*arg)[0] == '.' && (*arg)[1] == '.' && !(*arg)[2])))
	{
		put_multiple_error((char * [])
			{
			"cd",
			"error retrieving current directory", "getcwd",
			"cannot acess parent directories", NULL
		},
		"No such file or directory");
		string = string_duplicate(*arg);
		size = string_length(*arg);
		string_zero(*arg, size);
		realloc_appender(arg, env_get_value("PWD", env->envp));
		realloc_appender(arg, "/");
		realloc_appender(arg, string);
		if (string != NULL)
			free(string);
		return (0);
	}
	return (1);
}

int	change_paths(char *new_path, t_env *env, char *buffer, int cd_minus)
{
	char	*old_path;
	char	*newpath_alloc;
	char	*current_path;

	if (!new_path || !env || !env->envp)
		return (-1);
	newpath_alloc = string_duplicate(new_path);
	old_path = getcwd(buffer, PATH_MAX);
	if (!old_path)
		old_path = env_get_value("PWD", env->envp);
	env_update(env, "OLDPWD", "=", old_path);
	if (chdir(newpath_alloc) == -1)
		return (free(newpath_alloc), perror("chdir"), -1);
	if (cd_minus == true)
		string_print(newpath_alloc, true);
	current_path = getcwd(buffer, PATH_MAX);
	if (!current_path)
	{
		check_dot_edgecase(&newpath_alloc, env);
		current_path = newpath_alloc;
	}
	env_update(env, "PWD", "=", current_path);
	free(newpath_alloc);
	return (0);
}

char	*get_new_path(t_cmd *node, char **envp, int *cd_minus)
{
	char	*new_path;
	char	*new_path;

	if (!node || !envp)
		return (NULL);
	if (!node->args[1])
	{
		new_path = env_get_value("HOME", envp);
		if (!new_path)
			return (put_comand_error(node->args[0], "HOME not set"), NULL);
	}
	else if (node->args[1][0] == '-' && (node->args[1][1] == '\0'))
	{
		new_path = env_get_value("OLDPWD", envp);
		if (!new_path || !new_path[0])
			return (write(1, "cd: OLDPWD not set\n", 19), NULL);
		*cd_minus = 1;
	}
	else
		new_path = node->args[1];
	return (new_path);
}

int	built_cd(t_all *all, t_cmd *node, t_env *env, char *buffer)
{
	int		line;
	char	*new_path;
	int		cd_minus;
	int		line;
	char	*new_path;
	int		cd_minus;

	if (!all || !env || !env->envp || !node || !node->args)
		return (-1);
	line = -1;
	while (node->args[++line] != NULL)
		if (line > 1)
		return (put_comand_error(node->args[0], "too many arguments"), -1);
	cd_minus = 0;
	new_path = get_new_path(node, env->envp, &cd_minus);
	if (new_path && access(new_path, F_OK | X_OK) != 0)
	{
		put_multiple_error((char * [])
			{
			"cd", new_path, NULL
		}, strerror(errno));
		return (-1);
	}
	return (change_paths(new_path, env, buffer, cd_minus));
}
