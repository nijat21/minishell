/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   allocation_free.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olacerda <olacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 12:26:28 by olacerda          #+#    #+#             */
/*   Updated: 2026/03/11 12:55:52 by olacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "built-ins.h"

int	free_array_string(char **array, int size)
{
	int	line;

	if (!array)
		return (0);
	line = 0;
	while (array[line] && (((size) && (line < size)) || array[line]))
	{
		free(array[line]);
		line++;
	}
	// if (size == 0)
	// 	free(array[line]); //free NULL ??
	free(array);
	return (1);
}

void	del(void *content)
{
	if (!content)
		return ;
	free(content);
}