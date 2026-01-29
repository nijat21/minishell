#ifndef LEXER_H
#define LEXER_H

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdbool.h>

#include "Libft/libft.h"

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

typedef struct s_seg {
	char *val;
	bool expand;
	struct s_seg *next;
} t_seg;

typedef struct s_token {
	t_ttype type;
	t_seg *seg_list;
	struct s_token *next;
} t_token;


void *tokenise(void);
// Utils
void free_seg_list(t_seg **seg);
void free_token_list(t_token **tk);
void *safe_malloc(size_t bytes);
void print_token_list(t_token *tk);
bool is_space(char c);
bool is_operator(const char c);
void quote_context(const char c, t_quote *qc);
void choose_ttype(const char *str, t_ttype *tt);
size_t varname_len(const char *str);
// token operations
t_seg **add_segment(t_seg **seg, const char *val, size_t len, bool expand);
t_token **add_token(t_token **tk, t_ttype type, t_seg *seg_list);





#endif
