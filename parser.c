#include "parser.h"

/* Parser layers ->

	1. Check for syntax errors for all commands
		- yes -> return error msg
	2. Contstruct command pipeline
		- Expand exp=true segments
	3. Handle heredoc
		- exp=true if del has_quote=false && if $VAR in heredoc input
		- save into commands pipeline

	OUTPUT structure:
	?
*/

/*
	WORD("cat", expand=false)
	WORD("file.txt", expand=false)
	PIPE
	WORD("grep", expand=false)
	WORD("$var", expand=true)
	REDIR_OUT
	WORD("out.txt", expand=false)

	command 1.
	comand = "cat"
	args = ["cat", "file.txt", NULL]
	redir = NULL
	next → command 2

	command 2.
	comand = "grep"
	args = ["grep", "some", NULL]
	redir ->
		t_redirection
		type = REDIR_OUTPUT
		redir_arg = "out.txt"
		next = NULL
	next = NULL

	INTERESTING CASES:
	echo "some" > input > output
*/

/*
	Examples:
	cat intput
	echo some > input
	echo some > input > output
	echo some > input | cat >> output
*/

// REMOVE HAS QUOTE PROPERY FROM t_seg AS HEREDOC IS HANDLED AFTER PARSING NOW

void *parse_tokens(t_token *tk, int *exit_status)
{
	int res;
	// t_token *res_tk;
	// t_comand *res_tc;

	// SYNTAX CHECK
	res = syntax_check(tk);
	if (res)
	{
		*exit_status = res;
		return NULL;
	}
	// PIPELINE BUILDER
	// print_token_list(tk);
	print_comand(build_pipeline(tk));

	// res_tc = build_pipeline(tk);
	// if (!res_tc)
	// {
	// 	// free_token_list
	// 	return NULL;
	// }
	// // HEREDOC
	// res_tk = heredoc(tk, exit_status);
	// if (!res_tk)
	// {
	// 	// *exit_status = 1; // generic exit code if something else went wrong
	// 	printf("heredoc\n");
	// 	return NULL;
	// }

	// expander
	// convert to command pipelines
	return NULL;
}
