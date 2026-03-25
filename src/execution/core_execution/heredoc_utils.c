/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otlacerd <otlacerd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 10:40:34 by olacerda          #+#    #+#             */
/*   Updated: 2026/03/25 06:08:22 by otlacerd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <core_execution.h>
#include <libft.h>
#include <parser.h>

int	count_heredocs(t_cmd *head)
{
	t_cmd	*node;
	t_redir	*redir;
	int		count;
	t_cmd	*node;
	t_redir	*redir;
	int		count;

	if (!head)
		return (0);
	count = 0;
	node = head;
	while (node != NULL)
	{
		redir = node->redir;
		while (redir != NULL)
		{
			if (redir->type == REDIR_HEREDOC)
				count++;
			redir = redir->next;
		}
		node = node->next;
	}
	return (count);
	return (count);
}

void	unlink_all_heredoc_temps(char **heredoc_temps)
void	unlink_all_heredoc_temps(char **heredoc_temps)
{
	int	line;
	int	line;

	if (!heredoc_temps || !(*heredoc_temps))
		return ;
		return ;
	line = 0;
	while (heredoc_temps[line] != NULL)
	{
		unlink(heredoc_temps[line]);
		line++;
	}
}

char	*create_heredoc_temp_name(int index, char *std_name)
char	*create_heredoc_temp_name(int index, char *std_name)
{
	char	*name;
	char	*index_str;
	int		size;
	char	*name;
	char	*index_str;
	int		size;

	if (!std_name)
		return (NULL);
	index_str = int_to_ascii(index);
	size = 0;
	size += string_length(std_name);
	size += string_length(index_str);
	size += 1;
	name = malloc((size + 1) * sizeof(char));
	if (!name)
		return (NULL);
	string_copy(name, std_name);
	string_cat(name, (size + 1), "_");
	string_cat(name, (size + 1), index_str);
	free(index_str);
	return (name);
}

char	**create_heredoc_temps_buffer(int size)
{
	char	**result;
	char	**result;
	int		line;
	int		heredoc_identifier;

	if (size <= 0)
		return (NULL);
	result = malloc((size + 1) * sizeof(char *));
	if (!result)
		return (NULL);
	line = 0;
	heredoc_identifier = 0;
	while (line < size)
	{
		result[line] = create_heredoc_temp_name(heredoc_identifier, STD_TEMP);
		if (!result[line])
			return (free_array_string(result, line), NULL);
		heredoc_identifier++;
		if (access(result[line], F_OK) == 0)
		{
			free(result[line]);
			continue ;
			continue ;
		}
		line++;
	}
	return (result[line] = NULL, result);
}

int	add_heredoc_history(char *buffer, char *user_line, int size, char *path)
int	add_heredoc_history(char *buffer, char *user_line, int size, char *path)
{
	t_gal	x;
	t_gal	x;

	x = (t_gal){0, 0, 1, size, user_line, NULL, open(path, O_RDONLY)};
	(void)(((x.fd < 0) || !buffer || !user_line) && (x.readbytes = -1));
	while (x.readbytes > 0)
	{
		x.readbytes = read(x.fd, buffer, BUFFER_SZ);
		if (x.readbytes == 0)
			break ;
			break ;
		x.all_read += x.readbytes;
		(void)((x.readbytes > 0) && (x.new_line = malloc((x.all_read + 1))));
		if (!x.new_line || (x.readbytes == -1))
			return (free(x.line), 0);
		x.index1 = -1;
		while (++x.index1 < (x.all_read + 1) && (x.line) && x.line[x.index1])
			x.new_line[x.index1] = x.line[x.index1];
		free((char *)((long)x.line * (x.index2 > 0)));
		x.index2 = 0;
		while (x.index1 < (x.all_read + 1) && (x.index2 < x.readbytes))
			x.new_line[x.index1++] = buffer[x.index2++];
		x.new_line[x.index1] = '\0';
		x.line = x.new_line;
	}
	return (add_history(x.line),
		free((char *)((long)x.line * (x.index2 > 0))), close(x.fd), 1);
}
