/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otlacerd <otlacerd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 07:35:01 by olacerda          #+#    #+#             */
/*   Updated: 2026/03/25 04:42:05 by otlacerd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <utils.h>

void	put_error(char *problem)
{
	int	size;

	if (!problem)
		return ;
	size = 0;
	while (problem[size])
		size++;
	write(STDERR_FILENO, problem, size);
}

void	put_comand_error(char *comand, char *problem)
{
	put_error(comand);
	put_error(": ");
	put_error(problem);
	write(STDERR_FILENO, "\n", 1);
}

void	put_multiple_error(char **elements, char *problem)
{
	int	line;

	if (!elements && !problem)
		return ;
	line = 0;
	while (elements && elements[line])
	{
		put_error(elements[line]);
		put_error(": ");
		line++;
	}
	put_error(problem);
	write(STDERR_FILENO, "\n", 1);
}
