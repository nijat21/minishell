#include <parser.h>

void *free_arr_cmdlst(t_cmd **cmd, char **args)
{
    free_arr(args);
    command_lstclear(cmd);
    return NULL;
}

void *free_arg_cmdlst(t_cmd **cmd, char *arg)
{
    free(arg);
    command_lstclear(cmd);
    return NULL;
}

static void seg_val_expand(char **str, t_seg *seg, t_all *all)
{
    char *var;
    char *new_str;

    if (seg->expand)
    {
        var = expand_var(seg->val, all);
        new_str = ft_strjoin(*str, var);
        if (var)
            free(var);
        if (!new_str)
        {
            free(*str);
            *str = NULL;
            return;
        }
    }
    else
        new_str = ft_strjoin(*str, seg->val);
    free(*str);
    *str = new_str;
}

char *seg_to_str(t_seg *seg, t_all *all)
{
    char *str;

    str = NULL;
    while (seg)
    {
        if (!(seg->val))
        {
            ft_putstr_fd("Parser: seg->value NULL\n", 2);
            free(str);
            return NULL;
        }
        seg_val_expand(&str, seg, all);
        if (!str)
        {
            ft_putstr_fd("Parser: seg_to_str\n", 2);
            return NULL;
        }
        seg = seg->next;
    }
    return str;
}

char **word_tokens_to_args(t_token **tk, t_all *all)
{
    char **args;
    char *str;
    size_t i;
    size_t len;

    len = count_word_tokens(*tk);
    args = safe_malloc(sizeof(char *) * (len + 1), "word_tokens_to_args");
    if (!args)
        return NULL;
    i = -1;
    while ((*tk) && (*tk)->type == WORD)
    {
        str = seg_to_str((*tk)->seg_list, all);
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
