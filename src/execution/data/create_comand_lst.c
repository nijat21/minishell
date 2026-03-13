/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_comand_lst.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olacerda <olacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 09:13:47 by olacerda          #+#    #+#             */
/*   Updated: 2026/03/11 12:37:10 by olacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <data.h>

char	**create_args_buffer(int line, char **all_args)
{
	char 	**args;
	int		count;
	int		size;

	size = 0;
	args = NULL;
	while((all_args[line]) && (all_args[line][0] != '|'))
	{
		if (is_redirection(all_args[line]))
		{
			count = 0;
			while ((count < 2) && (all_args[line] != NULL))
			{
				count++;
				line++;
			}
		}
		else if (all_args[line++] != NULL)
			size++;
	}
	args = malloc((size + 1) * sizeof(char *));
	if (!args)
		return (NULL);
	return (args);
}

void	assign_args(int *line, char **splitted, char **comand, char **args)
{
	int	string;
	int	count;

	if (!line || !splitted || !comand || !args)
		return ;
	string = 0;
	*comand = splitted[(*line)];
	if (splitted[*line] == NULL)
		return ;
	while ((splitted[*line] != NULL) && (splitted[*line][0] != '|'))
	{
		if (is_redirection(splitted[*line]) > 0)
		{
			count = 0;
			while ((count < 2) && (splitted[(*line)++] != NULL))
				count++;
		}
		else if (splitted[*line] != NULL)
		{
			args[string] = splitted[(*line)];
			(*line)++;
			string++;
		}
	}
	args[string] = NULL;
}

int	create_node_comand(t_cmd **head, t_cmd **node, char *cmd, char **args)
{
	if (!head || !node || !cmd || !args)
		return (0);
	*node = NULL;
	if ((*head) == NULL)
	{
		*head = comand_lstnew(cmd, args);
		if (!(*head))
			return (0);
		*node = *head;
	}
	else
	{
		*node = comand_lstnew(cmd, args);
		if (!(*node))
			return (comand_lstclear(&(*head), del), 0);
		comand_lstadd_back(&(*head), *node);
	}
	return (1);
}

int	create_linked_list(char **splitted, t_cmd **head, t_all *all)
{
	t_cmd *node;
	int	line;
	int	line_temp;
	char *comand;
	char **args;

	*head = NULL;
	comand = NULL;
	args = NULL;
	line = 0;
	if (!all || !splitted || !(*splitted) || !head)
		return (0);
	while (splitted[line] != NULL)
	{
		args = create_args_buffer(line, splitted);
		line_temp = line;
		assign_args(&line, splitted, &comand, args);
		if (create_node_comand(&(*head), &node, comand, args) == true)
			all->lst_size++;
		line = create_lst_redir(node, splitted, line_temp, &(all->heredoc_count));		
		if((splitted[line]) && (splitted[line][0] == '|'))
			free(splitted[line++]);
	}
	return (free(splitted), all->node_number = 0, 1);
}
