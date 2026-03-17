/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_heredoc_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otlacerd <otlacerd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 10:40:34 by olacerda          #+#    #+#             */
/*   Updated: 2026/03/17 00:46:03 by otlacerd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <core_execution.h>

void unlink_all_heredoc_temps(char **heredoc_temps)
{
	int line;

	if (!heredoc_temps)
		return;
	line = 0;
	while (heredoc_temps[line] != NULL)
	{
		unlink(heredoc_temps[line]);
		line++;
	}
}

char *create_heredoc_temp_name(int index, int father_pid, char *std_name)
{
	char *name;
	char *father_pid_str;
	char *index_str;
	int size;

	if (!std_name)
		return (NULL);
	father_pid_str = int_to_ascii(father_pid);
	index_str = int_to_ascii(index);
	size = 0;
	size += string_length(std_name);
	size += string_length(father_pid_str);
	size += string_length(index_str);
	size += 2; // For two under_lines between strings;
	name = malloc((size + 1) * sizeof(char));
	if (!name)
		return (NULL);
	string_copy(name, std_name);
	string_cat(name, (size + 1), "_"); // here
	string_cat(name, (size + 1), father_pid_str);
	string_cat(name, (size + 1), "_"); // and here
	string_cat(name, (size + 1), index_str);
	free(father_pid_str);
	free(index_str);
	return (name);
}

char	**create_heredoc_temps_buffer(int size, int father_pid)
{
	char **result;
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
		result[line] = create_heredoc_temp_name(heredoc_identifier, father_pid, STD_TEMP_LOCATION);
		if (!result[line])
			return (free_array_string(result, line), NULL);
		heredoc_identifier++;
		if (access(result[line], F_OK) == 0)
		{
			free(result[line]);
			continue;
		}
		line++;
	}
	return (result[line] = NULL, result);
}

int add_heredoc_history(char *buffer, char *user_line, int size, char *path)
{
	t_gal x;

	x = (t_gal){0, 0, 1, size, user_line, NULL, open(path, O_RDONLY)};
	if ((x.fd < 0) || !buffer || !user_line)
		return (0);
	while (x.readbytes > 0)
	{
		x.readbytes = read(x.fd, buffer, BUFFER_SZ);
		if (x.readbytes == 0)
			break;
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
	return (add_history(x.line), free((char *)((long)x.line * (x.index2 > 0))), close(x.fd), 1);
}

int read_write_content(char *end_mark, int stdin_backup, int fd, int *sig)
{
	char *line;

	if (!end_mark)
		return (0);
	while (1)
	{
		line = readline("> ");
		if (((line) && (string_compare(line, end_mark) == 0)) || ((!line) || !(*line) || (*sig == SIGINT)))
		{
			if (*sig == SIGINT)
				dup2(stdin_backup, STDIN_FILENO);
			if (line != NULL)
				free(line);
			break;
		}
		else if (line && *line)
			add_history(line);
		if ((line != NULL) && (*line != '\0'))
			(write(fd, line, string_length(line)), write(fd, "\n", 1));
		if (line != NULL)
			free(line);
	}
	rl_clear_history();
	return (1);
}

int	exec_heredoc_content(t_all *all, int *signal, char *end_marker, int fd)
{
	int		pid;
	int		stdin_backup;

	if (!end_marker || fd == -1 || !all || !signal)
		return (-1);
	pid = fork();
	if (pid == -1)
		return (-1);
	else if (pid == 0)
	{
		all->process_info->is_heredoc = true;
		stdin_backup = dup(all->fds->std_backup[0]);
		read_write_content(end_marker, stdin_backup, fd, signal);
		close(stdin_backup);
		close(fd);
		free(all->main_line);
		end_structures(all, true, true, 0);
	}
	else if (pid > 0)
	{
		all->process_info->is_heredoc = false;
		waitpid(pid, NULL, 0);
		return (pid);
	}
	return (1);
}
