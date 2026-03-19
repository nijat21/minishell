/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otlacerd <otlacerd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 13:10:09 by olacerda          #+#    #+#             */
/*   Updated: 2026/03/19 05:50:42 by otlacerd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <built-ins.h>

int	built_pwd(t_all *all, t_cmd *node, t_env *env, char *buffer)
{
	char *pwd_pointer;
	
	if (!buffer)
		return (-1);
	(void)env;
	(void)node;
	(void)env;
	(void)all;
	pwd_pointer = getcwd(buffer, PATH_MAX);
	if (!pwd_pointer)
		return (-1);
	string_print(pwd_pointer);
	write(1, "\n", 1);
	return (0);
}