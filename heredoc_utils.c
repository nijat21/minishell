#include "parser.h"

static char *delimiter(t_token *here_tk, bool *exp)
{
    t_seg *temp_seg;
    char *del;

    *exp = true;
    del = NULL;
    temp_seg = here_tk->seg_list;
    while (temp_seg)
    {
        if (temp_seg->has_quote == true)
            *exp = false;
        if (temp_seg->expand == true)
            temp_seg->expand = false;
        del = ft_strjoin(del, temp_seg->val);
        temp_seg = temp_seg->next;
    }
    return del;
}

static bool tokenize(t_lex_ctx *ctx, const char *str, int *i, bool exp)
{
    if (ctx->len == 0)
        ctx->start = &str[*i];
    if (exp && str[*i] == '$')
    {
        if (handle_var(ctx, &str[*i], i))
            return (true);
        else
            ctx->len++;
    }
    else if (is_space(str[*i]))
    {
        add_leftovers(ctx);
        if (ctx->len <= 0)
            return (true);
    }
    else
        ctx->len++;
    return (false);
}

static t_token *heredoc_lexer(t_token **tk, const char *str, bool exp)
{
    t_lex_ctx ctx;
    int i;

    ctx.tk = *tk;
    ctx.seg = NULL;
    ctx.tt = WORD;
    ctx.qc = Q_NONE;
    ctx.len = 0;
    ctx.has_quote = false;
    i = -1;
    while (str[++i])
        tokenize(&ctx, str, &i, exp);
    handle_last_buf(&ctx);
    return (ctx.tk);
}

t_token *handle_heredoc(t_token *here_tk)
{
    char *del;
    bool exp;
    char *prompt;
    t_token *new_tk;

    new_tk = NULL;
    del = delimiter(here_tk, &exp);
    printf("delimiter -> %s ", del);
    printf("exp -> %s\n", exp ? "yes" : "no");
    while (1)
    {
        prompt = readline("heredoc> ");
        new_tk = heredoc_lexer(&new_tk, prompt, exp);
        if (ft_strncmp(prompt, del, ft_strlen(del) + 1) == 0)
        {
            free(prompt);
            break;
        }
        free(prompt);
    }
    return new_tk;
}
