#include "parser.h"

/* Parser layers ->

	1. Check for syntax errors for all commands
		- yes -> return error msg
	2. Handle heredoc
		- exp=true if del has_quote=false && if $VAR in heredoc input
	3. Expand exp=true segments
	4. Add expanded commands into the pipeline and return

	OUTPUT structure:
	?
*/

void *parse_tokens(t_token *tk, int *exit_status)
{
	int res;

	res = syntax_check(tk);
	if (res)
	{
		*exit_status = res;
		return NULL;
	}
	// heredoc
	// expander
	// convert to command pipelines
	return NULL;
}
