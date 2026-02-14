#include "minishell.h"

// CONSTRAINTS -->
// Only one GLOBAL variable to store signal received
//
// FEATURES -->
// Display a prompt when waiting for a command
// Working history
// Search & Launch right executable based on path

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

int	main(void)
{
	char	*prompt;
	t_token	*tk;

	while (1)
	{
		prompt = readline("minishell> ");
		if (!prompt)
		{
			printf("exit\n");
			break ;
		}
		/* printf("prompt -> %s\n", prompt); */
		tk = lexer(prompt);
		if (!tk)
			printf("Include proper input\n");
		else
			print_token_list(tk);
		free(prompt);
	}
	return (0);
}
