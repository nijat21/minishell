/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nismayil <nismayil@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 11:59:44 by nismayil          #+#    #+#             */
/*   Updated: 2026/03/25 03:46:20 by nismayil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include "parse_structures.h"
# include "parser.h"

// ===== lexer.c =========================================================
void		ctx_init(t_lex_ctx *ctx);
t_token		*lexer(const char *prompt);

// ===== lexer_quotes.c ==================================================
int			handle_quotes(t_lex_ctx *ctx, const char *str, int *i);

// ===== lexer_utils.c ===================================================
void		free_seg_list(t_seg **seg);
void		free_token_list(t_token **tk);
void		*safe_malloc(size_t bytes, char *func_name);
bool		is_space(char c);
bool		is_operator(const char c);

// ===== lexer_context.c =================================================
void		quote_context(const char c, t_quote *qc);
void		choose_ttype(const char *str, t_ttype *tt);
size_t		varname_len(const char *str);
int			add_var(t_lex_ctx *ctx, const char *start, int *i);

// ===== token_ops.c =================================================
t_seg		*seg_init(t_lex_ctx *ctx, size_t len);
t_seg		**add_segment(t_lex_ctx *ctx, const char *val, size_t len,
				bool expand);
t_token		**add_token(t_token **tk, t_ttype type, t_seg *seg_list);

// ===== lexer_list_ops.c =================================================
t_lex_ctx	*if_len_add_seg(t_lex_ctx *ctx, bool exp);
t_lex_ctx	*if_len_add_token_seg(t_lex_ctx *ctx, t_ttype tt, bool exp);
t_lex_ctx	*handle_last_buf(t_lex_ctx *ctx);
t_lex_ctx	*add_leftovers(t_lex_ctx *ctx);

// ===== print_token.c =================================================
void		print_token_list(t_token *tk);

#endif
