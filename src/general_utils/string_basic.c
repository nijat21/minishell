/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_basic.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otlacerd <otlacerd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 07:25:51 by olacerda          #+#    #+#             */
/*   Updated: 2026/03/23 11:24:55 by otlacerd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <utils.h>

int	string_length(char *string)
{
	int	size;

	size = 0;
	if (!string)
		return (0);
	while (string[size])
		size++;
	return (size);
}

int	string_print(char *string, int have_break_line)
{
	int	size;

	if (!string)
		return (0);
	size = 0;
	while (string[size])
		size++;
	write(1, string, size);
	if (have_break_line == true)
		write(1, "\n", 1);
	return (1);
}

int	string_copy(char *str1, char *str2)
{
	int	index;

	if (!str1 || !str2)
		return (0);
	index = 0;
	while (str2[index] != '\0')
	{
		str1[index] = str2[index];
		index++;
	}
	str1[index] = '\0';
	return (1);
}

int	string_zero(char *string, int size)
{
	int	index;

	if (!string || (size <= 0))
		return (0);
	index = 0;
	while (index < size)
	{
		string[index] = '\0';
		index++;
	}
	return (1);
}

int	string_have_equal(char *string)
{
	int	index;

	if (!string)
		return (-1);
	index = 0;
	while (string[index])
	{
		if (string[index] == '=')
			return (1);
		index++;
	}
	return (0);
}
