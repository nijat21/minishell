/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   allocation_free.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nismayil <nismayil@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 12:26:28 by olacerda          #+#    #+#             */
/*   Updated: 2026/03/25 03:30:35 by nismayil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "built_ins.h"

int free_array_string(char **array, int size)
{
	int line;

	if (!array)
		return (0);
	line = 0;
	while (array[line] && (((size) && (line < size)) || array[line]))
	{
		free(array[line]);
		line++;
	}
	free(array);
	return (1);
}

void del(void *content)
{
	if (!content)
		return;
	free(content);
}