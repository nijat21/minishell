/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otlacerd <otlacerd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 07:21:00 by olacerda          #+#    #+#             */
/*   Updated: 2026/03/16 22:05:55 by otlacerd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include <utils.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include "../../minishell.h"

static char *liner(t_gnl *gnl, int *total_end, char **line)
{
	char 	*newline;
	int		index;

	newline = NULL;
	while ((gnl->end < gnl->readbytes) && (gnl->buffer[gnl->end] != '\n'))
	{
		gnl->end++;
		(*total_end)++;
	}
	newline = malloc((*total_end + (gnl->buffer[gnl->end] == '\n') + 1));
	if (!newline)
		return (free(*line), NULL);
	index = 0;
	while ((index < (*total_end - gnl->end)) && (newline) && (*line))
	{
		newline[index] = (*line)[index];
		index++;
	}
	while ((gnl->start <= gnl->end) && (gnl->start < gnl->readbytes) && newline)
	{
		newline[index] = gnl->buffer[gnl->start];
		index++;
		gnl->start++;
	}
	return (newline[index] = '\0', free(*line), newline);
}

char *get_next_line(int fd)
{
	static	t_gnl gnl[FD_SETSIZE];
	char *line;
	int	total_end;

	if ((BUFFER_SZ <= 0) || (fd < 0))
		return (NULL);
	if ((gnl[fd].readbytes <= 0) || (gnl[fd].start >= gnl[fd].readbytes))
		gnl[fd] = (t_gnl){BUFFER_SZ, 0, BUFFER_SZ, {0}};
	total_end = 0;
	line = NULL;
	while (((gnl[fd].buffer[gnl[fd].end] != '\n') || (line == NULL))
		&& (gnl[fd].readbytes > 0))
	{
		if (gnl[fd].start >= gnl[fd].readbytes)
		{
			gnl[fd].readbytes = read(fd, gnl[fd].buffer, BUFFER_SZ);
			if (gnl[fd].readbytes == -1)
				return (free(line), NULL);
			if (gnl[fd].readbytes == 0)
				break ;
			*(long *)&(gnl[fd].start) = 0;
		}
		line = liner(&gnl[fd], &total_end, &line);
	}
	return (gnl[fd].end += (gnl[fd].buffer[gnl[fd].end] == '\n'), line);
}
