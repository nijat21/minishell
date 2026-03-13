#include <parser.h>

t_tctx ttype_to_tctx(t_ttype t_tt)
{
    t_tctx res;

    res = T_NONE;
    if (t_tt == REDIR_IN || t_tt == REDIR_OUT || t_tt == HEREDOC || t_tt == APPEND)
        res = T_REDIRS;
    else if (t_tt == WORD)
        res = T_WORD;
    else if (t_tt == PIPE)
        res = T_PIPE;
    else if (t_tt == UNCLOSED_QUOTE)
        res = T_UNCLOSED_QUOTE;
    return res;
}

static int syntax_error(t_token *curr_tk, t_tctx *t_ctx)
{
    t_tctx curr;

    curr = ttype_to_tctx(curr_tk->type);
    if (curr == T_UNCLOSED_QUOTE)
        return 1;
    else if (*t_ctx == T_NONE)
    {
        if (curr == T_PIPE)
            return 2;
    }
    else if (*t_ctx == T_PIPE)
    {
        if (curr == T_PIPE)
            return 3;
    }
    else if (*t_ctx == T_REDIRS)
    {
        if (curr == T_PIPE || curr == T_REDIRS)
            return 4;
    }
    *t_ctx = curr;
    return 0;
}

int syntax_check(t_token *tk)
{
    t_tctx t_ctx;
    t_token *temp;
    int res;

    t_ctx = T_NONE;
    temp = tk;
    while (temp)
    {
        res = syntax_error(temp, &t_ctx);
        if (res)
        {
            if (res == 1)
                print_unclosed_quote();
            else
                print_syntax_error(temp->seg_list->val);
            return EXIT_MISUSE;
        }
        temp = temp->next;
    }
    if (t_ctx == T_PIPE || t_ctx == T_REDIRS)
    {
        print_syntax_error("newline");
        return EXIT_MISUSE;
    }
    return 0;
}
