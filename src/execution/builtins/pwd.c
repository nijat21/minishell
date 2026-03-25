/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nismayil <nismayil@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 13:10:09 by olacerda          #+#    #+#             */
/*   Updated: 2026/03/25 07:05:07 by nismayil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <built_ins.h>

int	built_pwd(t_all *all, t_cmd *node, t_env *env, char *buffer)
{
	char	*pwd_pointer;

	if (!buffer)
		return (-1);
	(void)node;
	(void)all;
	pwd_pointer = getcwd(buffer, PATH_MAX);
	if (!pwd_pointer)
	{
		pwd_pointer = env_get_value("PWD", env->envp);
		if (pwd_pointer == NULL)
			return (perror("getcwd"), -1);
	}
	string_print(pwd_pointer, true);
	env_update(env, "PWD", "=", pwd_pointer);
	return (0);
}
