#include "minishell.h"

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

// There's some trash value here for some reason even after assignment
int g_signal = 0;

void sigint_main(int sig)
{
	g_signal = sig;
	ioctl(STDIN_FILENO, TIOCSTI, "\n"); // kernel 6.2+ dropped TIOCSTI + Async-signal-safe concern
										// ft_putchar_fd('\n', STDOUT_FILENO);
										// rl_on_new_line();
										// rl_redisplay();
}

void setup_main_signals(void)
{
	struct sigaction sa;

	rl_catch_signals = 0;
	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = sigint_main;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	sa.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa, NULL);
}

int main(void)
{
	char *line;
	t_token *tk;
	int exit_status;

	exit_status = 0;
	setup_main_signals();
	while (1)
	{
		line = readline("minishell> ");
		if (g_signal == SIGINT)
		{
			printf("Ctrl-c\n");
			exit_status = 130;
			g_signal = 0;
			free(line);
			continue;
		}
		if (!line)
		{
			ft_putstr_fd("exit\n", STDERR_FILENO);
			exit(exit_status);
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
			break;
		parse_tokens(tk, &exit_status);
		// if (exit_status)
		// 	free_token_list(&tk);
	}
	if (tk)
		print_token_list(tk);

	free_token_list(&tk);
	// clear everything
	// rl_clear_history();
	return (0);
}
