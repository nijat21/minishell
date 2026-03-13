/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olacerda <olacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 07:35:01 by olacerda          #+#    #+#             */
/*   Updated: 2026/03/05 00:20:13 by olacerda         ###   ########.fr       */
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
	if (!comand)
		return ;
	put_error(comand);
	put_error(": ");
	put_error(problem);
	write(STDERR_FILENO, "\n", 1);
}