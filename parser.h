#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

void *parse_tokens(t_token *tk);

// util
void print_syntax_error(t_token *tk);


#endif
