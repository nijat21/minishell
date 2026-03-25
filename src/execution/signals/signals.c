/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otlacerd <otlacerd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 06:58:17 by olacerda          #+#    #+#             */
/*   Updated: 2026/03/25 07:13:35 by nismayil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signals.h>

t_proc	*get_process_info(t_all *all)
{
	static t_proc	*process_info;

	if (all != NULL)
		process_info = all->process_info;
	return (process_info);
}

void	handler(int sig)
{
	int				pid;
	static t_proc	*process_info;

	if (process_info == NULL)
		process_info = get_process_info(NULL);
	if (sig == SIGINT)
	{
		process_info->signal = sig;
		process_info->exit_status = 130;
		if (process_info->is_heredoc == true)
		{
			pid = waitpid(-1, NULL, 0);
			if (pid == -1)
				((1) && (write(STDOUT_FILENO, "\n", 1), close(STDIN_FILENO)));
		}
		if (process_info->is_heredoc == false)
		{
			write(STDOUT_FILENO, "\n", 1);
			rl_replace_line("", 1);
			rl_on_new_line();
			rl_redisplay();
		}
	}
}

void	signals(int have_child, int pid)
{
	if (have_child == true)
	{
		if (pid == CHILD)
		{
			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_DFL);
		}
		else if (pid > 0)
		{
			signal(SIGINT, SIG_IGN);
			signal(SIGQUIT, SIG_IGN);
		}
	}
	else
	{
		signal(SIGQUIT, SIG_IGN);
		signal(SIGINT, handler);
	}
}
