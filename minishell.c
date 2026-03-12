/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nismayil <nismayil@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 12:00:16 by nismayil          #+#    #+#             */
/*   Updated: 2026/03/12 09:38:00 by nismayil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

/*
	MINISHELL LOOP   ← SIG HANDLING
	↓
	INPUT
	↓
	LEXER
	↓
	PARSER
		↓
		SYNTAX_CHECK
		↓
		HEREDOC PROCESSING  ← PROMPTING & SIG HANDLING
		↓
		VARIABLE EXPANSION
		↓
		CONSTRUCT COMMAND PIPELINE
	↓
	EXECUTOR  ← command not found, execve errors
*/

int g_signal = 0;

int main(void)
{
	char *line;
	int exit_status;
	t_token *tk;
	t_comand *cmd;

	exit_status = 0;
	setup_signals();
	while (1)
	{
		line = readline("minishell> ");
		if (g_signal == SIGINT)
		{
			exit_status = EXIT_SIGINT;
			g_signal = 0;
			free(line);
			continue;
		}
		if (!line)
		{
			ft_putstr_fd("exit\n", STDERR_FILENO);
			break;
		}
		if (!(*line))
		{
			free(line);
			continue;
		}
		add_history(line);
		tk = lexer(line);
		free(line);
		if (!tk)
		{
			exit_status = EXIT_FAILURE;
			free_token_list(&tk);
			break;
		}
		// print_token_list(tk);
		cmd = parse_tokens(tk, &exit_status);
		free_token_list(&tk);
		if (!cmd)
			break;
		print_comand(cmd);
	}

	// clear everything
	rl_clear_history();
	return (0);
}
