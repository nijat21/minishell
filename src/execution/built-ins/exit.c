/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otlacerd <otlacerd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 00:56:52 by otlacerd          #+#    #+#             */
/*   Updated: 2026/03/19 05:49:22 by otlacerd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <built-ins.h>

int is_overflow_long(char *string)
{
	char *reference;
	int	long_len;
	int	string_len;

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

int	parse_exit(char **args)
{
	int	index;
	int line;

	if (!args || !(*args) || !(args[1]))
		return (0);
	line = 1;
	while (args[line] != NULL)
		line++;
	if (line > 2)
		return (put_comand_error(args[0], "too many arguments"), 0);
	index = 0;
	while (args[1][index])
	{
		if (is_numerical(args[1][index]) == false)
			return (-1);
		index++;
	}
	if (is_overflow_long(args[1]) == true)
		return (-1);
	return (1);
}

int	built_exit(t_all *all, t_cmd *node, t_env *env, char *buffer)
{
	int		result;
	long	number;

	if (!node || !node->args || !all)
		return (-1);
	(void)env;
	(void)buffer;
	write(STDERR_FILENO, "exit\n", 5);
	result = parse_exit(node->args);
	if (result == 0)
		return (-1);
	else if (result == -1)
	{
		put_error(node->args[0]);
		put_error(": ");
		put_comand_error(node->args[1], "numeric argument required");
	}
	number = ascii_to_long(node->args[0]);
	end_structures(all, true, false, number % 256);
	return (0);
}


