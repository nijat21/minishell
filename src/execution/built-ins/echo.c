/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nismayil <nismayil@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 01:54:57 by olacerda          #+#    #+#             */
/*   Updated: 2026/03/23 16:24:23 by nismayil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <built-ins.h>

int parse_echo(char **args, int *line)
{
	int index;

	if (!args || !(*args))
		return (false);
	while (args[(*line)] != NULL)
	{
		if (!(*(short *)args[*line] == *(short *)'-n'))
			break;
		else
		{
			index = 2;
			while (args[*line][index])
			{
				if (args[*line][index] != 'n')
					return (false);
				index++;
			}
		}
		(*line)++;
	}
	return (0);
}

int built_echo(t_all *all, t_cmd *node, t_env *env, char *buffer)
{
	int line;
	int flag;

	if (!node || !node->args)
		return (-1);
	(void)env;
	(void)buffer;
	(void)all;
	line = 1;
	flag = 0;

	if (node->args[line] && (*(short *)node->args[line] == *(short *)'-n'))
		flag = parse_echo(node->args, &line);
	else
		flag = false;
	while (node->args[line])
	{
		string_print(node->args[line]);
		line++;
		if (node->args[line])
			write(STDOUT_FILENO, " ", 1);
	}
	if (flag == false)
		write(STDOUT_FILENO, "\n", 1);
	return (0);
}
