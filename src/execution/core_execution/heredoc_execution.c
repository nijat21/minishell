/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_execution.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nismayil <nismayil@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/17 21:47:32 by otlacerd          #+#    #+#             */
/*   Updated: 2026/03/18 17:29:30 by nismayil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core_execution.h"
#include "parser.h"

int exec_all_heredocs(t_all *all)
{
	t_cmd *node;
	t_redir *redirection;
	int index;

	if (!all)
		return (0);
	node = all->head;
	all->heredoc_count = count_heredocs(all->head);
	all->heredoc_temps = create_heredoc_temps_buffer(all->heredoc_count, all->father_pid);
	index = 0;
	while (node != NULL)
	{
		redirection = node->redir;
		while (redirection != NULL)
		{
			if (redirection->type == REDIR_HEREDOC)
			{
				all->process_info->signal = 0;
				exec_heredoc(all, redirection, all->heredoc_temps, index);
				index++;
			}
			redirection = redirection->next;
		}
		node = node->next;
	}
	return (0);
}

int exec_heredoc(t_all *all, t_redir *redir, char **temps, int index)
{
	static int fd;
	int readbytes;

	if (!all || !redir)
		return (FAIL);
	fd = open(temps[index], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		return (0);
	exec_heredoc_content(all, &(all->process_info->signal), redir, fd);
	close(fd);

	// Decide if we gonna keep this "history" ----------------------------------------------------------
	fd = open(temps[index], O_RDONLY);
	readbytes = read(fd, all->buffer, BUFFER_SZ);
	close(fd);
	if (readbytes != 0)
	{
		realloc_appender(&all->main_line, "\n");
		add_heredoc_history(all->buffer, all->main_line, string_length(all->main_line), temps[index]);
	}
	// -----------------------------------------------------------------------------------------------
	return (true);
}

int exec_heredoc_content(t_all *all, int *signal, t_redir *redir, int fd)
{
	int pid;
	int stdin_backup;

	if (!redir->redir_arg || fd == -1 || !all || !signal)
		return (-1);
	all->process_info->is_heredoc = true;
	pid = fork();
	if (pid == -1)
		return (-1);
	else if (pid == 0)
	{
		stdin_backup = dup(all->fds->std_backup[0]);
		read_write_content(all, redir, stdin_backup, fd);
		close(stdin_backup);
		close(fd);
		free(all->main_line);
		end_structures(all, true, true, 0);
	}
	else if (pid > 0)
	{
		waitpid(pid, NULL, 0);
		all->process_info->is_heredoc = false;
		return (pid);
	}
	return (1);
}

static void handle_sigint_eof(t_all *all, t_redir *redir, int stdin_backup, char *line)
{
	if (all->process_info->signal == SIGINT)
		dup2(stdin_backup, STDIN_FILENO);
	else if (!line)
		print_heredoc_eof_warning(redir->redir_arg);
	if (line)
		free(line);
}

int read_write_content(t_all *all, t_redir *redir, int stdin_backup, int fd)
{
	char *line;

	if (!redir->redir_arg)
		return (0);
	while (1)
	{
		line = readline("> ");
		if ((line && (string_compare(line, redir->redir_arg) == 0)) || (!line || (all->process_info->signal == SIGINT)))
		{
			handle_sigint_eof(all, redir, stdin_backup, line);
			break;
		}
		else if (line && !*line)
			write(fd, "\n", 1);
		else if (line && *line)
		{
			add_history(line);
			if (!expand_redir_var(redir, all, fd, line))
				all->process_info->exit_status = EXIT_FAILURE;
			if (line != NULL)
				free(line);
		}
	}
	rl_clear_history();
	return (1);
}
