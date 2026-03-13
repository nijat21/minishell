/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olacerda <olacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 05:29:14 by otlacerd          #+#    #+#             */
/*   Updated: 2026/03/04 20:23:20 by olacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <built-ins.h>

int	built_unset(t_all *all, t_cmd *node, t_env *env, char *buffer)
{
	int	line;

	if (!node || !node->args || !env)
		return (0);
	line = 0;
	(void)buffer;
	(void)all;
	while (node->args[line])
	{
		env_remove(env, node->args[line]);
		line++;
	}
	return (1);
}
