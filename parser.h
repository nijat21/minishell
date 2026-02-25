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

// Functionalities
void *parse_tokens(t_token *tk, int *exit_status);
int syntax_check(t_token *tk);

// util
void print_syntax_error(const char *str);
void print_unclosed_quote();

#endif
