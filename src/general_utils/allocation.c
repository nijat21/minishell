/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   allocation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olacerda <olacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 07:27:33 by olacerda          #+#    #+#             */
/*   Updated: 2026/03/07 15:42:59 by olacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <utils.h>

void	*re_allocker(void *pointer, long size, long new_size, long type_size)
{
	void 	*new_pointer;
	long 	x1;
	long	x2;

	if (!pointer || !new_size || !type_size)
		return (NULL);
	new_pointer = malloc(new_size * type_size);
	if (!new_pointer)
		return (NULL);
	size = size * type_size;
	new_size = new_size * type_size;
	x1 = -1;
	while (++x1 < new_size)
		((char *)new_pointer)[x1] = '\0';
	x1 = -1;
	x2 = -1;
	while ((++x1 < new_size) && (++x2 < size))
		((char *)new_pointer)[x1] = ((char *)pointer)[x2];
	free(pointer);
	return (new_pointer);
}

int	realloc_appender(char **string, char *content)
{
	int	string_size;
	int	new_size;
	
	if (!string || !(*string))
		return (-1);
	new_size = 0;
	string_size = string_length(*string);
	new_size += string_size;
	new_size += string_length(content);
	*string = re_allocker(*string, (string_size + 1), (new_size + 1), sizeof(char));
	if (!(*string))
		return (printf("oi\n"), 0);
	string_cat(*string, (new_size + 1), content);
	return (1);
}

char *string_duplicate(char *string)
{
	char *result;
	int	index;
	int	size;

	if (!string)
		return (NULL);
	size = string_length(string);
	result = malloc((size + 1) * sizeof(char));
	if (!result)
		return (NULL);
	index = 0;
	while (string[index])
	{
		result[index] = string[index];
		index++;
	}
	result[index] = '\0';
	return (result);
}

static char	**assign_split_line(char *string, int count, int index2, char **result)
{
	int index1;
	int line;

	index1 = 0;
	line = 0;
	while (string[index1] != '\0')
	{
		count = 0;
		while ((string[index1] != '\0') && (string[index1] == ' '))
			index1++;
		if ((string[index1] != '\0') && (string[index1] != ' '))
		{
			index2 = index1;
			while ((string[index1] != '\0') && (string[index1++] != ' '))
				count++;
			result[line] = malloc((count + 1) * (sizeof(char)));
			if (!result[line])
				return (free_array_string(result, line), NULL);
			count = 0;
			while ((string[index2] != '\0') && (string[index2] != ' '))
				result[line][count++] = string[index2++];
			result[line++][count] = '\0';
		}
	}
	return (result[line] = NULL, result);
}

char **split_line(char *string)
{
	char **result;
	int	index1;
	int	count;

	if (!string)
		return (NULL);
	index1 = 0;
	count = 0;
	while (string[index1] != '\0')
	{
		while ((string[index1] != '\0') && (string[index1] == ' '))
			index1++;
		if ((string[index1] != '\0') && (string[index1] != ' '))
		{
			count++;
			while ((string[index1] != '\0') && (string[index1] != ' '))
				index1++;
		}
	}
	if (count <= 0)
		return (NULL);
	result = malloc((count + 1) * (sizeof(char *)));
	if (!result)
		return (NULL);
	return (assign_split_line(string, count, 0, result));
}
