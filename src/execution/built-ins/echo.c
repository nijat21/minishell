/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otlacerd <otlacerd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 01:54:57 by olacerda          #+#    #+#             */
/*   Updated: 2026/03/16 22:13:43 by otlacerd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <built-ins.h>

int	parse_echo(char **args, int *line)
{
	int	index;

	if (!args || !(*args))
		return (FAIL);
	while (args[(*line)] != NULL)
	{
		if (*(short *)args[(*line)] != *(short *)"-n")
		{
			if (*line == 1)
				return (false);
			break ;
		}
		else
		{
			index = 2;
			while (args[(*line)][index])
			{
				if (!((args[(*line)][index] != '\0') && (args[(*line)][index] == 'n')))
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
		return (0);
	(void)env;
	(void)buffer;
	(void)all;
	line = 1;
	flag = 0;
	if ((line == 1) && (*(short *)node->args[line] == *(short *)"-n"))
		flag = parse_echo(node->args, &line);
	while (node->args[line] != NULL)
	{
		string_print(node->args[line]);
		line++;
		if (node->args[line] != NULL)
			write(STDOUT_FILENO, " ", 1);
	}
	if (flag == false)
		write(STDOUT_FILENO, "\n", 1);
	return (1);
}
