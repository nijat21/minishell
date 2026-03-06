#include "parser.h"

/*
    echo some$user >> out.txt
    [WORD]echo [WORD]some, $user [APPEND]>> [WORD]out.txt

    cat << del in > out
    [WORD]cat [HEREDOC]<< [WORD]del [WORD]in [REDIR_OUT]> [WORD]out

    command 1.
    comand = "echo"
    args = ["echo", "some$user(expanded)", NULL]
    redir ->
        t_redirection
        type = APPEND
        redir_arg = "out.txt"
        next = NULL
    next → NULL


    cat << EOF1 file.txt > out.txt >> log.txt < in.txt
    hello world
    EOF1

    WORD(cat)
    HEREDOC(<<)
    WORD(EOF1)
    WORD(file.txt)
    REDIR_OUTPUT(>)
    WORD(out.txt)
    REDIR_APPEND(>>)
    WORD(log.txt)
    REDIR_INPUT(<)
    WORD(in.txt)

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

char *expand_var()
{
    return NULL;
}

char *seg_to_str(t_seg *seg)
{
    char *str;

    str = NULL;
    while (seg)
    {
        if (!(seg->val))
        {
            printf("Parser: seg->value NULL\n");
            free(str);
            return NULL;
        }
        if (seg->expand)
            printf("expand\n"); // expanding function
        str = ft_strjoin(str, seg->val);
        if (!str)
        {
            ft_putstr_fd("Parser: ft_strjoin\n", 2);
            free(str);
            return NULL;
        }
        seg = seg->next;
    }
    return str;
}

char **word_tokens_to_args(t_token **tk)
{
    char **args;
    char *str;
    size_t i;

    args = safe_malloc(sizeof(char *) * count_word_tokens(*tk) + 1);
    if (!args)
        return NULL;
    i = -1;
    while ((*tk) && (*tk)->type == WORD)
    {
        str = seg_to_str((*tk)->seg_list);
        if (!str)
        {
            free_arr(args);
            return NULL;
        }
        args[++i] = str;
        if (!(*tk)->next || ((*tk)->next && (*tk)->next->type != WORD))
            break;
        (*tk) = (*tk)->next;
    }
    args[++i] = NULL;
    return args;
}

t_comand *word_tokens_to_cmd(t_comand **cmd, t_token **tk)
{
    char **new_args;
    t_comand *new;

    new_args = word_tokens_to_args(tk);
    if (!new_args)
        return NULL;
    if (*cmd)
    {
        (*cmd)->args = join_args((*cmd)->args, new_args);
        if (!(*cmd)->args)
        {
            command_lstclear(cmd);
            return NULL;
        }
    }
    else
    {
        new = command_lstnew(new_args[0], new_args);
        if (!new)
            return NULL;
        *cmd = new;
    }
    return *cmd;
}

// Redir token will for sure have a next token that's it's argument
t_redirection *token_to_redir(t_comand **cmd, t_token **tk)
{
    t_redirection *new_red;
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
    new_red = redir_lstnew(type, arg);
    if (!new_red)
        return NULL;
    redir_lstadd_back(&((*cmd)->redir), new_red);
    return (*cmd)->redir;
}

// Memory management and correct error propagation
// Restructure
// Var expansion
// Heredoc
t_comand *build_pipeline(t_token *tk)
{
    t_comand *cmd;
    t_comand *new;

    cmd = NULL;
    new = NULL;
    while (tk)
    {
        if (tk->type == WORD)
        {
            new = word_tokens_to_cmd(&new, &tk);
            if (!new)
                return NULL;
        }
        else if (ttype_to_tctx(tk->type) == T_REDIRS)
        {
            if (!token_to_redir(&new, &tk))
                return NULL;
        }
        else if (tk->type == PIPE)
        {
            command_lstadd_back(&cmd, new);
            new = NULL;
        }
        tk = tk->next;
    }
    if (new)
        command_lstadd_back(&cmd, new);
    return cmd;
}
