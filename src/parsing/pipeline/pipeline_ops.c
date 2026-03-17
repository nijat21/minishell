#include <parser.h>
#include <lexer.h>

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

void udpate_seg(char **str, char *seg_val)
{
    char *new_str;

    new_str = ft_strjoin(*str, seg_val);
    free(*str);
    *str = new_str;
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
            udpate_seg(&str, expand_var(seg->val));
        else
            udpate_seg(&str, seg->val);
        if (!str)
        {
            ft_putstr_fd("Parser: ft_strjoin_free\n", 2);
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

    args = safe_malloc(sizeof(char *) * (count_word_tokens(*tk) + 1));
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
