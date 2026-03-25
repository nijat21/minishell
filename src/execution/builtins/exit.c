/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otlacerd <otlacerd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 00:56:52 by otlacerd          #+#    #+#             */
/*   Updated: 2026/03/24 23:57:03 by otlacerd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <built-ins.h>

int parse_exit(char **args)
{
	int index;
	int line;

	if (!args || !(*args))
		return (-1);
	line = 1;
	if (!(args[line]))
		return (1);
	while (args[line] != NULL)
		if (++line > 2)
			return (put_comand_error(args[0], "too many arguments"), -1);
	string_trim(&args[1], args[1], (char []){' ', '	', '\0'});
	if (args[1] && (!(args[1][0]) || (args[1][0] == ' ') || (args[1][0] == '	')))
		return (-3);
	if (have_space_between(args[1]) == true)
		return (-3);
	index = -1;
	if (args[1] && ((args[1][0] == '+') || (args[1][0] == '-')))
		index++;
	while ((args[1]) && args[1][++index])
		if (is_numerical(args[1][index]) == false)
			return (-3);
	if ((args[1]) && is_overflow_long(args[1]) == true)
		return (-3);
	return (1);
}

static int	get_exit_code(int exit_code, int is_child, int error)
{
	int result;

	if ((is_child == false) && (error < 0))
		exit_code = -exit_code;
	if (exit_code != 0)
		result = exit_code;
	else
		result = error;
	return (result);
}

int built_exit(t_all *all, t_cmd *node, t_env *env, char *buffer)
{
	int		is_child;
	int		result;
	static long	exit_number;

	exit_number = 0;
	is_child = false;
	if (all && all->children_pids[all->node_nbr] == 0)
		is_child = true;
	if (!node || !node->args || !all || !env || !buffer)
		return (get_exit_code(all->process_info->exit_status, is_child, -2));
	if (is_child == false)
		write(STDERR_FILENO, "exit\n", 5);
	result = parse_exit(node->args);
	if (result == -1)
		return (get_exit_code(all->process_info->exit_status, is_child, -1));
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
