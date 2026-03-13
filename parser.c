#include "includes/parser.h"

/* Parser layers ->
	1. Lexer tokenize all commands
		- handle quotes, vars
		- epand and has quote attributes saved
	2. Check for syntax errors for all tokens
		- yes -> return error msg
	3. Contstruct command pipeline
		- Expand exp=true segments
	4. Handle heredoc
		- should run in child and kill it in Ctrl+C
		- exp=true if del has_quote=false && if $VAR in heredoc input
		- save into commands pipeline
*/

t_comand *parse_tokens(t_token *tk, int *exit_status)
{
	int res;
	t_comand *cmd;

	// SYNTAX CHECK
	res = syntax_check(tk);
	if (res)
	{
		*exit_status = res;
		return NULL;
	}
	// PIPELINE BUILDER
	cmd = NULL;
	if (!build_pipeline(&cmd, tk))
	{
		*exit_status = EXIT_FAILURE;
		return NULL;
	}
	// HEREDOC
	set_signal(S_HEREDOC);
	*exit_status = heredoc(cmd);
	set_signal(S_PARENT);
	if (*exit_status)
		return NULL;
	printf("heredoc status -> %d\n", *exit_status);
	*exit_status = EXIT_SUCCESS;
	return cmd;
}
