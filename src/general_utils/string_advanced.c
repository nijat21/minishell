/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_advanced.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otlacerd <otlacerd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 07:30:23 by olacerda          #+#    #+#             */
/*   Updated: 2026/03/19 21:51:58 by otlacerd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <utils.h>

int	string_cat(char *string1, int size, char *string2)
{
	int	x1;
	int x2;
	
	if (!string1 || !string2)
		return (0);
	x1 = 0;
	while (string1[x1] != '\0')
		x1++;
	x2 = 0;
	while ((string2[x2] != '\0'))
	{
		if (x1 >= size)
			break;
		string1[x1] = string2[x2];
		x1++;
		x2++;
	}
	string1[x1] = '\0';
	return (1);
}

int	string_swap(char **str1, char **str2)
{
	char *temp;

	if (!str1 || !str2)
		return (0);
	temp = *str1;
	*str1 = *str2;
	*str2 = temp;
	return (1);
}

int	string_compare(char *str1, char *str2)
{
	int	i;

	if (!str1 || !str2)
		return (0);
	i = 0;
	while ((str1[i] && str2[i]) && (str1[i] == str2[i])
				&& (str1[i + 1] != '\n'))
		i++;
	return (str1[i] - str2[i]);
}

int	string_trim(char **string, char to_remove)
{
	char *result;
	int	size;
	int	start;
	int	end;

	if (!string || !(*string) || !to_remove)
		return (0);
	start = 0;
	while ((*string)[start] && ((*string)[start] == to_remove))
		start++;
	end = start;
	while ((*string)[end])
		end++;
	while ((end > 0) && (*string)[--end] == to_remove)
		;
	result = malloc(((end - start) + 1) * sizeof(char));
	if (!result)
		return (0);
	size = 0;
	while ((*string)[start] && (start <= end))
		result[size++] = (*string)[start++];
	return (result[size] = '\0', free(*string), *string = result, 1);
}

int	array_string_lenght(char **args)
{
	int	line;

	line = 0;
	while (args[line] != NULL)
		line++;
	return (line);
}