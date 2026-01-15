#ifndef LEXER_H
#define LEXER_H

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdbool.h>

typedef enum e_quote {
	Q_NONE,
	Q_SINGLE,
	Q_DOUBLE
} t_quote;

typedef enum e_ttype {
	WORD,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	HEREDOC,
	APPEND,
	UNCLOSED_QUOTE
} t_ttype;

typedef struct s_token {
	t_ttype type;
	char *value;
	bool expand;
	struct s_token *next;
} t_token;


void *tokenise(void);

#endif
