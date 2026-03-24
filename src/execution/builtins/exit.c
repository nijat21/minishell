/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nismayil <nismayil@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 00:56:52 by otlacerd          #+#    #+#             */
/*   Updated: 2026/03/24 20:48:17 by nismayil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <built-ins.h>

int parse_exit(char **args)
{
	int index;
	int line;

	if (!args || !(*args))
		return (0);
	line = 1;
	if (!(args[line]))
		return (-1);
	while (args[line] != NULL)
		if (++line > 2)
			return (put_comand_error(args[0], "too many arguments"), 0);
	string_trim(&args[1], args[1], (char[]){' ', '	', '\0'});
	if (have_space_between(args[1]) == true)
		return -3;
	index = -1;
	while ((args[1]) && args[1][++index])
		if (is_numerical(args[1][index]) == false)
			return (-3);
	if ((args[1]) && is_overflow_long(args[1]) == true)
		return (-3);
	return (1);
}

int get_exit_code(int exit_code, int is_child)
{
	int result;

	if (exit_code > 0)
		result = exit_code;
	else
		result = 2;
	if (is_child == false)
		result = -result;
	return (result);
}

int built_exit(t_all *all, t_cmd *node, t_env *env, char *buffer)
{
	int is_child;
	int result;
	long exit_number;

	exit_number = 0;
	is_child = false;
	if (all && all->children_pids[all->node_nbr] == 0)
		is_child = true;
	if (!node || !node->args || !all || !env || !buffer)
		return (get_exit_code(all->process_info->exit_status, is_child));
	if (is_child == false)
		write(STDERR_FILENO, "exit\n", 5);
	result = parse_exit(node->args);
	if (result == 0)
		return (get_exit_code(all->process_info->exit_status, is_child));
	else if (result == -3)
	{
		put_multiple_error((char *[]){node->args[0], node->args[1], NULL},
						   "numeric argument required");
		exit_number = 2;
	}
	else if ((result == -1) && (node->args[1] == NULL))
		exit_number = all->process_info->exit_status;
	else if ((result == true) && (node->args[1] != NULL))
		exit_number = ascii_to_long(node->args[1]);
	return (end_structures(all, true, is_child, exit_number % 256), 0);
}
