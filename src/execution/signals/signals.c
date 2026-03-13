/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olacerda <olacerda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 06:58:17 by olacerda          #+#    #+#             */
/*   Updated: 2026/03/10 22:08:04 by olacerda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signals.h>

extern int	process_signal;

t_proc	*get_process_info(t_all *all)
{
	static t_proc *process_info;
	
	if (all != NULL)
		process_info = all->process_info;
	return (process_info);
}

void	handler(int sig)
{
	int		pid;
	t_proc	*process_info;
	
	if (sig == SIGINT)
	{
		process_info = get_process_info(NULL);
		process_info->signal = sig;
		process_info->exit_status = 130;
		if (process_info->is_heredoc == true)
			close(STDIN_FILENO);
		pid = waitpid(-1, NULL, 0);
		if ((pid == -1) && (process_info->is_heredoc == true))
			write(STDOUT_FILENO, "\n", 1);			
		if (((pid == -1) && (process_info->is_heredoc == false)))
		{
			write(STDOUT_FILENO, "\n", 1);
			rl_replace_line("", 1);
			rl_on_new_line();
			rl_redisplay(); 
		}
	}
}

void	signals(int is_child)
{
	if (is_child == true)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
	}
	else
	{
		signal(SIGQUIT, SIG_IGN);
		signal(SIGINT, handler);
	}
}
