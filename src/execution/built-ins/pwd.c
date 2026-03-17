/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otlacerd <otlacerd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 13:10:09 by olacerda          #+#    #+#             */
/*   Updated: 2026/03/16 22:13:43 by otlacerd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <built-ins.h>

int	built_pwd(t_all *all, t_cmd *node, t_env *env, char *buffer)
{
	char *pwd_pointer;
	
	(void)env;
	(void)node;
	(void)env;
	(void)all;
	pwd_pointer = getcwd(buffer, PATH_MAX);
	string_print(pwd_pointer);
	write(1, "\n", 1);
	return (1);
}