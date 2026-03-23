#include <parser.h>

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

static int lex(t_all *all, t_token **tk)
{
	char *line;
	size_t i;

	line = all->main_line;
	i = 0;
	if (line && !line[i])
		return 2;
	while (line[i] && is_space(line[i]))
		i++;
	if (!line[i])
		return 2;
	(*tk) = lexer(line);
	if (!(*tk))
	{
		all->process_info->exit_status = EXIT_FAILURE;
		free_token_list(tk);
		return 1;
	}
	return 0;
}

static bool valid_syntax(t_all *all, t_token *tk)
{
	int res;

	res = syntax_check(tk);
	if (res)
	{
		all->process_info->exit_status = res;
		return false;
	}
	return true;
}

t_parse_stat parse(t_all *all)
{
	t_cmd *cmd;
	t_token *tk;
	int res;

	res = lex(all, &tk);
	if (res == 1)
		return PARSE_FAIL;
	else if (res == 2)
		return BAD_INPUT;
	if (!valid_syntax(all, tk))
		return BAD_INPUT;
	cmd = NULL;
	if (!build_pipeline(&cmd, tk, all))
	{
		command_lstclear(&cmd);
		all->process_info->exit_status = EXIT_FAILURE;
		return PARSE_FAIL;
	}
	// all->process_info->exit_status = EXIT_SUCCESS; // OTO comented #################
	free_token_list(&tk);
	all->head = cmd;
	return PARSE_SUCCESS;
}
