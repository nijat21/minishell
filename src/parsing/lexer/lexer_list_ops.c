/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_list_ops.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nismayil <nismayil@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 19:09:07 by nismayil          #+#    #+#             */
/*   Updated: 2026/03/24 16:03:52 by nismayil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <lexer.h>

t_lex_ctx *if_len_add_seg(t_lex_ctx *ctx, bool exp)
{
	if (ctx->len > 0)
	{
		if (!add_segment(ctx, ctx->start, ctx->len, exp))
		{
			free_token_list(&ctx->tk);
			return NULL;
		}
		ctx->len = 0;
	}
	return ctx;
}

t_lex_ctx *if_len_add_token_seg(t_lex_ctx *ctx, t_ttype tt, bool exp)
{
	if (ctx->len > 0)
	{
		if (!add_segment(ctx, ctx->start, ctx->len, exp))
		{
			free_token_list(&ctx->tk);
			return NULL;
		}
		ctx->len = 0;
		if (!add_token(&(ctx->tk), tt, ctx->seg))
		{
			free_token_list(&ctx->tk);
			return NULL;
		}
		ctx->seg = NULL;
	}
	return ctx;
}

t_lex_ctx *handle_last_buf(t_lex_ctx *ctx)
{
	if (ctx->len > 0 || ctx->tt == UNCLOSED_QUOTE)
		if (!add_segment(ctx, ctx->start, ctx->len, false))
		{
			free_token_list(&ctx->tk);
			return NULL;
		}
	if (ctx->seg)
		if (!add_token(&ctx->tk, ctx->tt, ctx->seg))
		{
			free_token_list(&ctx->tk);
			return NULL;
		}
	return ctx;
}

t_lex_ctx *add_leftovers(t_lex_ctx *ctx)
{
	if (ctx->len == 0 && ctx->seg)
	{
		if (!add_token(&(ctx->tk), WORD, ctx->seg))
			return NULL;
		ctx->seg = NULL;
	}
	if (!if_len_add_token_seg(ctx, WORD, false))
		return NULL;
	return ctx;
}
