/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nismayil <nismayil@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 11:59:24 by nismayil          #+#    #+#             */
/*   Updated: 2026/03/16 15:11:24 by nismayil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <lexer.h>

static int handle_quote_var(t_lex_ctx *ctx, const char *str, int *i, char s_quote)
{
    int res;

    if (ctx->qc == Q_NONE)
    {
        if (!if_len_add_seg(ctx, false))
            return -1;
        if (str[*i - 1] && str[*i - 1] == s_quote)
            if (!add_segment(ctx, "", ctx->len, false))
                return -1;
        return 1;
    }
    if (ctx->qc == Q_DOUBLE && str[*i] == '$')
    {
        res = add_var(ctx, &str[*i], i);
        if (res)
            return 2;
        else if (res == -1)
            return -1;
    }
    if (ctx->len == 0)
        ctx->start = &str[*i];
    ctx->len++;
    return 0;
}

int handle_quotes(t_lex_ctx *ctx, const char *str, int *i)
{
    char s_quote;
    int res;

    s_quote = str[*i];
    if (!if_len_add_seg(ctx, false))
        return -1;
    ctx->has_quote = true;
    while (str[++(*i)] && ctx->qc != Q_NONE)
    {
        quote_context(str[(*i)], &(ctx->qc));
        res = handle_quote_var(ctx, str, i, s_quote);
        if (res == 1)
            break;
        else if (res == 2)
            continue;
        else if (res == -1)
            return -1;
    }
    if (ctx->qc != Q_NONE)
    {
        ctx->tt = UNCLOSED_QUOTE;
        return (1);
    }
    return (0);
}
