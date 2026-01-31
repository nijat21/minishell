#include "lexer.h"

/*
 	TOKENISATION ->

	Detect operators |, <, >, <<, >>
	Detect quotes '', "" (ignore unclosed quotes)
	Unsopported chars '; and  \'
	$?, $var segs exp=true unless '$var'
*/

static bool handle_var(t_lex_ctx *ctx, const char *start, int *i)
{
	size_t var_len;

	var_len = varname_len(start);
	if(var_len)
	{ 
		if_len_add_seg(ctx, false);
		add_segment(&(ctx->seg), start, var_len, true);
		*i += var_len - 1;
		return true;
	}
	return false;
}

static bool handle_quotes(t_lex_ctx *ctx, const char *str, int *i)
{
	if_len_add_seg(ctx, false);
	while(str[++(*i)] && ctx->qc != Q_NONE)
	{
		quote_context(str[(*i)], &(ctx->qc));
		if(ctx->qc == Q_NONE) 
		{
			if_len_add_seg(ctx, false);
			break; 
		}
		if(ctx->qc == Q_DOUBLE && str[*i] == '$')
			if(handle_var(ctx, &str[*i], i))
				continue;
		if(ctx->len == 0)
			ctx->start = &str[*i];
		ctx->len++;
	}
	if(ctx->qc != Q_NONE)
	{ 	
		ctx->tt = UNCLOSED_QUOTE; 
		return true;
	}
	return false;
}

static void handle_operator(t_lex_ctx *ctx, const char *str, int *i)
{
	if(ctx->len == 0 && ctx->seg)
	{
		add_token(&(ctx->tk), WORD, ctx->seg);
		ctx->seg = NULL;
	}				
	if_len_add_token_seg(ctx, WORD, false);
	ctx->start = &str[*i];
	ctx->len++;
	if(ctx->tt == HEREDOC || ctx->tt == APPEND)
	{
		ctx->len++;
		(*i)++;
	}
	if_len_add_token_seg(ctx, ctx->tt, false);
}

static bool handle_non_quote(t_lex_ctx *ctx, const char *str, int *i)
{
	if(ctx->len == 0)
		ctx->start = &str[*i];
	if(str[*i] == '$')
		if(handle_var(ctx, &str[*i], i))
			return true;	
	if(is_space(str[*i]))
	{
		if(ctx->len == 0 && ctx->seg)
		{
			add_token(&(ctx->tk), WORD, ctx->seg);
			ctx->seg = NULL;
		}				
		if_len_add_token_seg(ctx, WORD, false);
		if(ctx->len <= 0)
			return true;	
	}
	else if(is_operator(str[*i]))
		handle_operator(ctx, str, i);
	else
		ctx->len++;
	return false;
}

t_token *lexer(const char *str)
{
	t_lex_ctx ctx; 
	int i;

	ctx.tk = NULL;
	ctx.seg = NULL;
	ctx.len = 0;
	i = -1;
	while(str[++i])
	{
		quote_context(str[i], &ctx.qc);
		choose_ttype(&str[i], &ctx.tt);
		if(ctx.qc == Q_DOUBLE || ctx.qc == Q_SINGLE)
		{
			if(handle_quotes(&ctx, str, &i))
				break;
		}
		else
		{
			if(handle_non_quote(&ctx, str, &i))
				continue;
		}
	}
	handle_last_buf(&ctx);
	return ctx.tk;
}


