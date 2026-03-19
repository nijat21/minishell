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

#include <built-ins.h>

int	change_paths(char *new_path, int *oldpwd_status, t_env *env, char *buffer)
{
	char 			*old_path;

	if (!new_path || !env || !env->envp || !oldpwd_status)
		return (0);
	old_path = getcwd(buffer, PATH_MAX);
	chdir(new_path);
	if (*oldpwd_status == 0)
	{
		env_update(env, "OLDPWD", "=", old_path);
		(*oldpwd_status) = 1;
	}
	else if ((env_find_pointer("OLDPWD", env->envp) != NULL) && ((*oldpwd_status) == 1))
		env_add(env, env_find_line("OLDPWD", env->envp), "OLDPWD=", old_path);
	env_add(env, env_find_line("PWD", env->envp), "PWD=", getcwd(buffer, PATH_MAX));
	return (1);
}

int	cd_minus(int oldpwd_status, char **new_path, char **envp)
{
	if (!new_path || !envp)
		return (0);
	if (oldpwd_status == 0 || (env_find_line("OLDPWD", envp) < 0))
	{
		(*new_path) = NULL;
		write(1, "cd: OLDPWD not set\n", 19);
		return (0);
	}
	*new_path = env_get_value("OLDPWD", envp);
	if (access(*new_path, F_OK | X_OK) != 0)
		return (0);
	string_print(*new_path);
	write(1, "\n", 1);
	return (1);
}

int	built_cd(t_all *all, t_cmd *node, t_env *env, char *buffer)
{
	int			line;
	static int	oldpwd_status;
	char 		*new_path;

	if (!all || !env || !env->envp || !node || !node->args)
		return (-1);
	line = -1;
	while (node->args[++line] != NULL)
		if (line > 2)
			return (write(1, "cd: too many arguments\n", 23), -1);
	line = 1;
	if (!node->args[line])
		new_path = env_get_value("HOME", env->envp);
	else if ((node->args[line][0] == '-') && (node->args[line][1] == '\0'))
		cd_minus(oldpwd_status, &new_path, env->envp);
	else
		new_path = node->args[line];
	if (new_path != NULL)
	{
		if (access(new_path, F_OK | X_OK) != 0)
			return (perror("cd"), -1);
		change_paths(new_path, &oldpwd_status, env, buffer);
		return (0);
	}
	return (-1);
}
