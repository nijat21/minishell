/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_advanced.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olacerda <olacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 07:30:23 by olacerda          #+#    #+#             */
/*   Updated: 2026/03/04 09:01:29 by olacerda         ###   ########.fr       */
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

int	string_jump_words(int	quantity, char *string, int *index)
{
	int	count;

	if (!quantity || !string)
		return (-1);
	count = 0;
	while (count < quantity)
	{
		while ((string[*index]) && (string[*index] != ' ') && (string[*index] != '|'))
			(*index)++;
		count++;
		if (string[*index] == '\0')
			return (0) ;
		else if (string[*index] == ' ')
			(*index)++;
	}
	return (1);
}
int	array_string_lenght(char **args)
{
	int	line;

	line = 0;
	while (args[line] != NULL)
		line++;
	return (line);
}