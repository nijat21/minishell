/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core_execution_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olacerda <olacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 02:53:08 by otlacerd          #+#    #+#             */
/*   Updated: 2026/03/04 08:26:01 by olacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <core_execution.h>

int	is_accessible(char *path, char *comand)
{
	int	result;

	if (!path || !comand)
		return (0);
	result = 0;
	result = access((const char *)path, X_OK);
	if (result == 0)
		return (1);
	return (0);
}

int	compare_prefix(char *prefix, char *string)
{
	int	index;

	index = 0;
	if (!prefix || !string)
		return (0);
	while ((prefix[index]) && (string[index]) && prefix[index] == string[index])
	{
		index++;
	}
	if (prefix[index] == '\0')
	{
		return (1);
	}
	return (0);
}
