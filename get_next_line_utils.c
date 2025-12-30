/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nismayil <nismayil@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 17:46:05 by nismayil          #+#    #+#             */
/*   Updated: 2025/01/22 22:57:54 by nismayil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int gnl_strlen(char *str)
{
	int count;

	if (!str)
		return (0);
	count = 0;
	while (str[count] && str[count] != '\n')
		count++;
	if (str[count] == '\n')
		count++;
	return (count);
}

void ft_strcpy(char *des, char *src)
{
	int i;

	i = 0;
	while (src[i])
	{
		des[i] = src[i];
		i++;
	}
	des[i] = '\0';
	i++;
	while (des[i])
	{
		des[i] = '\0';
		i++;
	}
	return;
}

char *make_line(char *str, int *index)
{
	char *new_line;
	int len;
	int i;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	*index = len;
	new_line = malloc(len + 1);
	if (!new_line)
		return (NULL);
	i = 0;
	while (i < len && str[i])
	{
		new_line[i] = str[i];
		i++;
	}
	new_line[i] = '\0';
	return (new_line);
}

char *gnl_strjoin(char *prev_line, char *curr_line)
{
	char *final_line;
	char *start;

	if (!curr_line)
		return (NULL);
	final_line = malloc(ft_strlen(prev_line) + ft_strlen(curr_line) + 1);
	if (!final_line)
		return (NULL);
	start = final_line;
	if (prev_line)
		while (*prev_line)
			*final_line++ = *prev_line++;
	if (curr_line)
		while (*curr_line)
			*final_line++ = *curr_line++;
	*final_line = '\0';
	return (start);
}

void create_merge(char **final_line, char *buf)
{
	char *new_line;
	char *temp_line;
	int index;

	index = 0;
	new_line = make_line(buf, &index);
	if (!new_line)
	{
		free(*final_line);
		*final_line = NULL;
		return;
	}
	temp_line = ft_strjoin(*final_line, new_line);
	free(new_line);
	free(*final_line);
	if (!temp_line)
	{
		*final_line = NULL;
		free(temp_line);
		return;
	}
	*final_line = temp_line;
	ft_strcpy(buf, &buf[index]);
}
