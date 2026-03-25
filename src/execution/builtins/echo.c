/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otlacerd <otlacerd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 01:54:57 by olacerda          #+#    #+#             */
/*   Updated: 2026/03/24 21:48:16 by otlacerd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <built-ins.h>

static int	flag_is_true(char *string)
{
	if (!string)
		return (0);
	if (string[0] == '-' && (string[1] == 'n'))
		return (true);
	return (false);
}

int	parse_echo(char **args, int *line)
{
	int	index;

	if (!args || !(*args))
		return (FAIL);
	while (args[(*line)] != NULL)
	{
		if (flag_is_true(args[*line]) == false)
			break ;
		else
		{
			index = 2;
			while (args[(*line)][index])
			{
				if (!((args[(*line)][index] != '\0')
						&& (args[(*line)][index] == 'n')))
					return (false);
				index++;
			}
		}
		(*line)++;
	}
	return (true);
}

int	built_echo(t_all *all, t_cmd *node, t_env *env, char *buffer)
{
	int	line;
	int	flag;

	if (!node || !node->args)
		return (-1);
	(void)env;
	(void)buffer;
	(void)all;
	flag = 0;
	line = 1;
	if (node->args[line] && (flag_is_true(node->args[line]) == true))
		flag = parse_echo(node->args, &line);
	while ((node->args[line] != NULL))
	{
		string_print(node->args[line], false);
		line++;
		if (node->args[line] != NULL)
			write(STDOUT_FILENO, " ", 1);
	}
	if (flag == false)
		write(STDOUT_FILENO, "\n", 1);
	return (0);
}
