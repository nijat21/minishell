/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otlacerd <otlacerd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 13:10:09 by olacerda          #+#    #+#             */
/*   Updated: 2026/03/23 09:32:25 by otlacerd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <built-ins.h>

int	built_pwd(t_all *all, t_cmd *node, t_env *env, char *buffer)
{
	char *pwd_pointer;
	
	if (!buffer)
		return (-1);
	(void)node;
	(void)all;
	pwd_pointer = getcwd(buffer, PATH_MAX);
	if (!pwd_pointer)
	{
		pwd_pointer = env_get_value("PWD", env->envp);
		if (pwd_pointer == NULL)
			return (0);
	}
	string_print(pwd_pointer, true);
	env_update(env, "PWD", "=", pwd_pointer);
	return (0);
}