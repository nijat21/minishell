/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nismayil <nismayil@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 05:29:14 by otlacerd          #+#    #+#             */
/*   Updated: 2026/03/25 03:30:35 by nismayil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <built_ins.h>

int built_unset(t_all *all, t_cmd *node, t_env *env, char *buffer)
{
	int line;

	if (!node || !node->args || !env)
		return (-1);
	line = 0;
	(void)buffer;
	(void)all;
	while (node->args[line])
	{
		env_remove(env, node->args[line]);
		line++;
	}
	return (0);
}
