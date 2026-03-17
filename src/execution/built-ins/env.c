/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otlacerd <otlacerd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 22:13:30 by otlacerd          #+#    #+#             */
/*   Updated: 2026/03/16 22:13:43 by otlacerd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "built-ins.h"

char	**create_env(char **envp, int *capacity)
{
	char **result;
	int	line_count;
	int	line;

	if (!capacity)
		return (NULL);
	line_count = 0;
	while ((envp) && envp[line_count])
		line_count++;
	*capacity = line_count + ENV_INCREMENT;
	result = malloc((*capacity + 1) * sizeof(char *));
	if (!result)
		return (NULL);
	line = 0;
	while (envp[line])
	{
		result[line] = string_duplicate(envp[line]);
		if (!result[line])
			return (free_array_string(result, line), NULL);
		line++;
	}
	result[line] = NULL;
	return (result);
}

int	assign_minimal_env(t_env *env, char *buffer)
{
	int		shell_lvl;	
	char	*pointer;

	if (!env)
		return (0);
	env_update(env, "PWD", "=", getcwd(buffer, PATH_MAX));
	if (env_find_pointer("OLDPWD", env->envp) ==  false)
		env_update(env, "OLDPWD", NULL, NULL);
	if (env_find_pointer("PATH", env->envp) == false)
		env_update(env, "PATH", "=", PATH_BACKUP);
	pointer = env_get_value("SHLVL", env->envp);
	shell_lvl = ascii_to_int(pointer);
	if (shell_lvl < 0)
		shell_lvl = 1;
	else if (shell_lvl >= 0)
		shell_lvl++;
	pointer = int_to_ascii(shell_lvl);
	if ((pointer != NULL) && (shell_lvl >= 0))
	{
		env_update(env, "SHLVL", "=", pointer);
		free(pointer);
	}
	else if ((pointer == NULL) || (shell_lvl < 0))
		env_update(env, "SHLVL", "=", "0");
	return (1);	
}

int	assign_env_struct(t_env *env, char **envp, char *buffer)
{
	int	capacity;
	
	if (!env || !buffer)
		return (write(2, "Error\nWrong pointer in function assign_env_struct\n", 50), 0);
	capacity = 0;
	env->envp = create_env(envp, &capacity);
	if (!env->envp)
		return (0);
	env->capacity = capacity;
	env->size = capacity - ENV_INCREMENT;
	assign_minimal_env(env, buffer);
	return (1);
}

int	env_show(char **envp, int is_export)
{
	int	line;
	int	size;

	if (!envp)
		return (0);
	line = -1;
	while (envp[++line])
	{
		(void)((is_export == true) && (write(1, "export ", 7)));
		if ((is_export == false) && string_have_equal(envp[line]) == false)
			continue ;
		size = 0;
		while (envp[line][size] && (envp[line][size] != '='))
			size++;
		if ((write(1, envp[line], size)) && (envp[line][size] == '='))
		{
			write(STDOUT_FILENO, &envp[line][size++], 1);
			(void)((is_export == true) && (write(STDOUT_FILENO, "\"", 1)));
			while(envp[line][size])
				write(1, &envp[line][size++], 1);
			(void)((is_export == true) && (write(STDOUT_FILENO, "\"", 1)));
		}
		write(1, "\n", 1);
	}
	return (1);
}

int	built_env(t_all *all, t_cmd *node, t_env *env, char *buffer)
{
	int	size;

	if (!env || !env->envp || !node || !node->args)
		return (0);
	(void)buffer;
	(void)all;
	size = 0;
	while (node->args[size] != NULL)	
		size++;
	if (size > 1)
	{
		put_error(node->args[0]);
		put_error(": ");
		put_error("‘");
		put_error(node->args[1]);
		put_comand_error("’", "No such file or directory");
	}
	else if (size == 1)
		env_show(env->envp, 0);
	return (1);	
}
