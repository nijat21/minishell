/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nismayil <nismayil@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 11:59:44 by nismayil          #+#    #+#             */
/*   Updated: 2026/03/13 19:04:40 by nismayil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
#define LEXER_H

#include "../src/parser/Libft/libft.h"
#include "minishell.h"

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <errno.h>

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

int handle_quotes(t_lex_ctx *ctx, const char *str, int *i);

// utils
void free_seg_list(t_seg **seg);
void free_token_list(t_token **tk);
void *safe_malloc(size_t bytes);
bool is_space(char c);
bool is_operator(const char c);

// context
void quote_context(const char c, t_quote *qc);
void choose_ttype(const char *str, t_ttype *tt);
int add_var(t_lex_ctx *ctx, const char *start, int *i);

// token operations
t_seg *seg_init(t_seg **seg, size_t len);
t_seg **add_segment(t_lex_ctx *ctx, const char *val, size_t len, bool expand);
t_token **add_token(t_token **tk, t_ttype type, t_seg *seg_list);

// lexer list operations
t_lex_ctx *if_len_add_seg(t_lex_ctx *ctx, bool exp);
t_lex_ctx *if_len_add_token_seg(t_lex_ctx *ctx, t_ttype tt, bool exp);
t_lex_ctx *handle_last_buf(t_lex_ctx *ctx);
t_lex_ctx *add_leftovers(t_lex_ctx *ctx);

// temp functions
void print_token_list(t_token *tk);
const char *qc_to_str(t_quote qc);

#endif
