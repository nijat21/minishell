/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otlacerd <otlacerd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 11:59:33 by nismayil          #+#    #+#             */
/*   Updated: 2026/03/16 22:43:48 by otlacerd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <parser.h>
#include <lexer.h>

static int handle_space_operator(t_lex_ctx *ctx, const char *str, int *i)
{
	if (is_space(str[*i]))
	{
		if (!add_leftovers(ctx))
			return -1;
		if (ctx->len == 0)
			return (1);
	}
	else if (is_operator(str[*i]))
	{
		if (!add_leftovers(ctx))
			return -1;
		ctx->start = &str[*i];
		ctx->len++;
		if (ctx->tt == HEREDOC || ctx->tt == APPEND)
		{
			ctx->len++;
			(*i)++;
		}
		if (!if_len_add_token_seg(ctx, ctx->tt, false))
			return -1;
	}
	return 0;
}

static bool handle_non_quote(t_lex_ctx *ctx, const char *str, int *i)
{
	int res;

	if (ctx->len == 0)
		ctx->start = &str[*i];
	if (str[*i] == '$')
	{
		res = add_var(ctx, &str[*i], i);
		if (res)
			return (true);
		else if (res == -1)
			return false;
		else
			ctx->len++;
	}
	else if (is_space(str[*i]) || is_operator(str[*i]))
	{
		res = handle_space_operator(ctx, str, i);
		if (res)
			return true;
		else if (res == -1)
			return false;
	}
	else
		ctx->len++;
	return (true);
}

static int input_to_tokens(t_lex_ctx *ctx, const char *str, int *i)
{
	int res;

	if (str[*i] == '"' || str[*i] == '\'')
	{
		res = handle_quotes(ctx, str, i);
		if (res)
			return 1;
		else if (res == -1)
			return -1;
	}
	else
	{
		ctx->has_quote = false;
		if (!handle_non_quote(ctx, str, i))
			return -1;
	}
	return 0;
}

t_token *lexer(const char *str)
{
	t_lex_ctx ctx;
	int i;
	int res;

	ctx.tk = NULL;
	ctx.qc = Q_NONE;
	ctx.seg = NULL;
	ctx.len = 0;
	ctx.has_quote = false;
	i = -1;
	while (str[++i])
	{
		quote_context(str[i], &ctx.qc);
		choose_ttype(&str[i], &ctx.tt);
		res = input_to_tokens(&ctx, str, &i);
		if (res)
			break;
		else if (res == -1)
			return NULL;
	}
	if (!handle_last_buf(&ctx))
		return NULL;
	return (ctx.tk);
}
