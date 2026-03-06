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

t_token *heredoc_lexer(t_token **tk, const char *str, bool exp)
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

void here_sigint_handler(int sig)
{
    g_signal = sig;
    // ft_putchar_fd('\n', STDOUT_FILENO);
    ioctl(STDIN_FILENO, TIOCSTI, "\n");
    rl_done = 1;
}

void setup_heredoc_signals(void)
{
    struct sigaction sa;

    rl_catch_signals = 0;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = here_sigint_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);
    sa.sa_handler = SIG_IGN;
    sigaction(SIGQUIT, &sa, NULL);
}

t_token *handle_heredoc(t_token *here_tk)
{
    char *del;
    bool exp;
    char *line;
    t_token *new_tk;

    new_tk = NULL;
    del = delimiter(here_tk, &exp);
    // printf("delimiter -> %s ", del);
    // printf("exp -> %s\n", exp ? "yes" : "no");
    setup_heredoc_signals();
    while (1)
    {
        line = readline("heredoc> ");
        if (g_signal == SIGINT)
        {
            free(line);
            break;
        }
        if (!line)
        {
            // free_seg_list
            break;
        }
        new_tk = heredoc_lexer(&new_tk, line, exp);
        if (ft_strncmp(line, del, ft_strlen(del) + 1) == 0)
        {
            free(line);
            break;
        }
        free(line);
    }
    return new_tk;
}

/*
    CONSIDER USING IOCTL IN MAIN SIGS AS WELL
    ERROR PROPAGATION
    PROPER CLEANING FOR VARIOUS CASES
*/

void push_token(t_token *tk, t_token *new_tk)
{
    t_token *tk_next;
    t_token *iter;

    tk_next = (tk)->next;
    (tk)->next = new_tk;
    iter = new_tk;
    while (iter->next)
        iter = iter->next;
    iter->next = tk_next;
}