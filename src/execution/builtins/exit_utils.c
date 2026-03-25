/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otlacerd <otlacerd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/21 08:44:03 by username          #+#    #+#             */
/*   Updated: 2026/03/25 03:17:51 by otlacerd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

static int	check_remove(char xar, char *set_remove)
{
	int	index;

	if (!set_remove)
		return (0);
	index = -1;
	while (set_remove[++index])
		if (set_remove[index] == xar)
		return (1);
	return (0);
}

int	string_trim(char **string, char *ref, char *set_remove)
{
	char	*result;
	int		x1;

	if (!string || !(*string) || !ref || !set_remove)
		return (0);
	while (*ref && (check_remove(*ref, set_remove) == 1))
		ref++;
	x1 = 0;
	while (ref[x1])
		x1++;
	while ((--x1 > 0) && (check_remove(ref[x1], set_remove) == 1))
		ref[x1] = '\0';
	result = malloc((x1 + 2) * sizeof(char));
	if (!result)
		return (0);
	x1 = -1;
	while (ref[++x1])
		result[x1] = ref[x1];
	return (result[x1] = '\0', free(*string), *string = result, 1);
}

int	is_overflow_long(char *string)
{
	char	*reference;
	int		long_len;
	int		string_len;

	if (!string)
		return (0);
	reference = MAX_LONG_STR;
	if (string[0] == '-')
		reference = MIN_LONG_STR;
	long_len = string_length(reference);
	string_len = string_length(string);
	if (string_len > long_len)
		return (1);
	if (string_len < long_len)
		return (0);
	else if (string_compare(string, reference) <= 0)
		return (0);
	return (1);
}

int	have_space_between(char *string)
{
	int	index;

	if (!string)
		return (0);
	index = 0;
	while (string[index])
	{
		if (string[index] == ' ')
		{
			while (string[index] == ' ')
			{
				if ((string[index] != ' ') && (string[index] != '\0'))
					return (true);
				index++;
			}
		}
		index++;
	}
	return (false);
}
