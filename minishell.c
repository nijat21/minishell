#include "minishell.h"

// CONSTRAINTS -->
// Only one GLOBAL variable to store signal received
//
// FEATURES -->
// Display a prompt when waiting for a command
// Working history
// Search & Launch right executable based on path

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

/*
	INPUT FOR EXECUTION:
	char *argv[] = {"echo", "-n", "something", NULL}

	DISTINCTIONS:

	builtin?
	yes
		pipeline?   -> fork
		no pipeline -> parent
	no -> fork + execve
*/

int main(void)
{
	char *prompt;
	t_token *tk;
	int exit_status;

	while (1)
	{
		prompt = readline("minishell> ");
		add_history(prompt);
		tk = lexer(prompt);
		free(prompt);
		if (!tk)
			continue;
		// print_token_list(tk);
		parse_tokens(tk, &exit_status);
		if (exit_status)
			free_token_list(&tk);
	}
	return (0);
}
