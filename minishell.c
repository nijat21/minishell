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

int main(void)
{
	char *prompt;
	t_token *tk;

	while (1)
	{
		write(1, "minishell> ", 11);
		prompt = readline("");
		if(!prompt)
		{
			perror("Read");
			return -1;
		}
		tk = lexer(prompt);
		if(!tk)
			printf("Include proper input\n");
		print_token_list(tk);
	}

   	return 0;
}
