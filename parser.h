#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

typedef enum e_tctx
{
	T_NONE,
	T_WORD,
	T_PIPE,
	T_REDIRS,
	T_UNCLOSED_QUOTE,
} t_tctx;

typedef enum e_redir_type
{
	REDIR_INPUT = 1,
	REDIR_OUTPUT = 2,
	REDIR_APPEND = 3,
	REDIR_HEREDOC = 4
} t_redir_type;

typedef struct s_redirection
{
	t_redir_type type;
	char *redir_arg;
	struct s_redirection *next;
} t_redirection;

typedef struct s_comand
{
	char *comand;
	char **args;
	t_redirection *redir;
	struct s_comand *next;
} t_comand;

// Functionalities
int syntax_check(t_token *tk);
t_token *handle_heredoc(t_token *here_tk);
t_token *heredoc(t_token *tk, int *exit_status);
void *parse_tokens(t_token *tk, int *exit_status);

// util
void print_syntax_error(const char *str);
void print_unclosed_quote();

#endif
