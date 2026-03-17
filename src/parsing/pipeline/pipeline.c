#include <parser.h>
#include <lexer.h>

/*
    cat << EOF1 file.txt > out.txt >> log.txt < in.txt
    hello world
    EOF1

    t_comand
    ┌─────────────────────────────┐
    │ comand = "cat"              │
    │ args = ["cat", "file.txt"]  │
    │ redir →                     │
    │   ├ type = REDIR_HEREDOC    │
    │   │ redir_arg = "EOF1"      │
    │   │ next →                  │
    │   ├ type = REDIR_OUTPUT     │
    │   │ redir_arg = "out.txt"   │
    │   │ next →                  │
    │   ├ type = REDIR_APPEND     │
    │   │ redir_arg = "log.txt"   │
    │   │ next →                  │
    │   └ type = REDIR_INPUT      │
    │     redir_arg = "in.txt"    │
    └─────────────────────────────┘
    next = NULL

*/

static t_cmd *word_tokens_to_cmd(t_cmd **cmd, t_token **tk)
{
    char **new_args;
    t_cmd *new;

    new_args = word_tokens_to_args(tk);
    if (!new_args)
    {
        command_lstclear(cmd);
        return NULL;
    }
    if (*cmd)
    {
        (*cmd)->args = join_args((*cmd)->args, new_args);
        if (!(*cmd)->args)
            return free_arr_cmdlst(cmd, new_args);
    }
    else
    {
        new = command_lstnew(new_args[0], new_args);
        if (!new)
            return free_arr_cmdlst(cmd, new_args);
        *cmd = new;
    }
    return *cmd;
}

static bool token_has_quote(t_seg *seg)
{
    while (seg)
    {
        if (seg->has_quote)
            return true;
        seg = seg->next;
    }
    return false;
}

static t_redir *token_to_redir(t_cmd **cmd, t_token **tk)
{
    t_redir *new_red;
    t_redir_type type;
    char *arg;

    new_red = NULL;
    if (!(*cmd))
    {
        (*cmd) = command_lstnew(NULL, NULL);
        if (!(*cmd))
            return NULL;
    }
    type = ttype_to_redir_type((*tk)->type);
    (*tk) = (*tk)->next;
    if (!(*tk))
        return NULL;
    arg = seg_to_str((*tk)->seg_list);
    if (!arg)
        return NULL;
    new_red = redir_lstnew(type, arg, token_has_quote((*tk)->seg_list));
    if (!new_red)
        return free_arg_cmdlst(cmd, arg);
    redir_lstadd_back(&((*cmd)->redir), new_red);
    return (*cmd)->redir;
}

t_cmd **build_pipeline(t_cmd **cmd, t_token *tk)
{
    t_cmd *curr;

    curr = NULL;
    while (tk)
    {
        if (tk->type == WORD)
        {
            curr = word_tokens_to_cmd(&curr, &tk);
            if (!curr)
                return NULL;
        }
        else if (ttype_to_tctx(tk->type) == T_REDIRS)
        {
            if (!token_to_redir(&curr, &tk))
                return NULL;
        }
        else if (tk->type == PIPE)
        {
            command_lstadd_back(cmd, curr);
            curr = NULL;
        }
        tk = tk->next;
    }
    command_lstadd_back(cmd, curr);
    return cmd;
}
