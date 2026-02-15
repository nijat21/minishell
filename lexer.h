#ifndef LEXER_H
#define LEXER_H

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdbool.h>
#include <errno.h>

#include "Libft/libft.h"

typedef enum e_quote
{
	Q_NONE,
	Q_SINGLE,
	Q_DOUBLE
} t_quote;

typedef enum e_ttype
{
	WORD,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	HEREDOC,
	APPEND,
	UNCLOSED_QUOTE,
} t_ttype;

typedef struct s_seg
{
	char *val;
	bool expand;
	bool has_quote;
	struct s_seg *next;
} t_seg;

typedef struct s_token
{
	t_ttype type;
	t_seg *seg_list;
	struct s_token *next;
} t_token;

typedef struct s_lex_ctx
{
	t_token *tk;
	t_seg *seg;
	const char *start;
	t_ttype tt;
	t_quote qc;
	size_t len;
	bool has_quote;
} t_lex_ctx;

// Utils
void free_seg_list(t_seg **seg);
void free_token_list(t_token **tk);
void *safe_malloc(size_t bytes);
bool is_space(char c);
bool is_operator(const char c);
void quote_context(const char c, t_quote *qc);
void choose_ttype(const char *str, t_ttype *tt);
size_t varname_len(const char *str);

// token operations
t_seg **add_segment(t_lex_ctx *ctx, const char *val, size_t len, bool expand);
t_token **add_token(t_token **tk, t_ttype type, t_seg *seg_list);
void if_len_add_seg(t_lex_ctx *ctx, bool exp);
void if_len_add_token_seg(t_lex_ctx *ctx, t_ttype tt, bool exp);
void handle_last_buf(t_lex_ctx *ctx);
void add_leftovers(t_lex_ctx *ctx);

// temp functions
void print_token_list(t_token *tk);
const char *qc_to_str(t_quote qc);

#endif
