/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otlacerd <otlacerd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 00:56:52 by otlacerd          #+#    #+#             */
/*   Updated: 2026/03/17 00:55:36 by otlacerd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <built-ins.h>

int is_bigger_than_long(char *string)
{
	int	long_len;
	int	string_len;

	if (!string)
		return (0);
	long_len = string_length(MAX_LONG_STR);
	string_len = string_length(string);
	if (string[0] == '-')
		string_len--;
	if (string_len > long_len)
		return (0);
	if (string_len < long_len)
		return (1);
	else if (string_compare(string, MAX_LONG_STR) < 0)
		return (0);
	return (1);
}

// int	parse_exit(char **args)
// {
// 	int line;
// 	int	count;

// 	if (!args || !(*args) || !(args[1]))
// 		return (FAIL);
// 	line = 1;
// 	while (args[line] != NULL)
// 	{
		
// 	}
// 	if (line == 1)
// 		return (1);
// }

int	built_exit(t_all *all, t_cmd *node, t_env *env, char *buffer)
{
	// int	line;

	if (!node || !node->args || !all)
		return (0);
	(void)env;
	(void)node;
	(void)buffer;
	write(STDERR_FILENO, "exit\n", 5);
	// line = 0;

	end_structures(all, true, false, 1);
	exit (1); // retornar o valor escrito em arg[x]
	return (1);
}
