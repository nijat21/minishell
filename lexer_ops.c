#include "lexer.h"

void	if_len_add_seg(t_lex_ctx *ctx, bool exp)
{
	if (ctx->len > 0)
	{
		add_segment(&(ctx->seg), ctx->start, ctx->len, exp);
		ctx->len = 0;
	}
}

void	if_len_add_token_seg(t_lex_ctx *ctx, t_ttype tt, bool exp)
{
	if (ctx->len > 0)
	{
		add_segment(&(ctx->seg), ctx->start, ctx->len, exp);
		ctx->len = 0;
		add_token(&(ctx->tk), tt, ctx->seg);
		ctx->seg = NULL;
	}
}

void	handle_last_buf(t_lex_ctx *ctx)
{
	if (ctx->len > 0 || ctx->tt == UNCLOSED_QUOTE)
		add_segment(&(ctx->seg), ctx->start, ctx->len, false);
	if (ctx->seg)
		add_token(&ctx->tk, ctx->tt, ctx->seg);
}
