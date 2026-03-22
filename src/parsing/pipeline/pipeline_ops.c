/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_ops.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nismayil <nismayil@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/20 19:25:21 by nismayil          #+#    #+#             */
/*   Updated: 2026/03/22 15:44:19 by nismayil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <parser.h>

static char **seg_expand_split(t_seg *seg, t_all *all, char **args)
{
    char *exp;
    char **var_args;

    exp = expand_var(seg->val, all);
    if (!exp)
        return NULL;
    if (!seg->has_quote)
    {
        var_args = ft_split(exp, ' ');
        free(exp);
        if (!ft_arrlen(var_args))
            return NULL;
        return ft_arrconcat_free(args, var_args);
    }
    var_args = add_str_to_last_arg(args, exp);
    free(exp);
    return var_args;
}

/*
    FOR EACH SEG:
        IF EXPAND:
            IF HAS_QUOTE:
                CONCAT EXPANDED VAR TO LAST ARG
            IF NOT:
                SPLIT EXPANDED VAR TO ARGS AND JOIN TO MAIN ARGS
        IF NOT:
            CONCAT SEG->VAL TO LAST ARG



    CHECK WITH EMTPY INPUT
*/
static char **seg_val_expand(char ***args, t_seg *seg, t_all *all)
{
    char **new_args;

    if (seg->expand)
        new_args = seg_expand_split(seg, all, *args);
    else
        new_args = add_str_to_last_arg(*args, seg->val);
    if (!new_args)
        return NULL;
    *args = new_args;
    return new_args;
}

static char **seg_to_args(t_seg *seg, t_all *all)
{
    char **args;

    args = NULL;
    while (seg)
    {
        if (!(seg->val))
        {
            ft_putstr_fd("seg_to_args: seg->value NULL\n", 2);
            ft_free_arr(args);
            return NULL;
        }
        args = seg_val_expand(&args, seg, all);
        if (!args || !ft_arrlen(args))
        {
            ft_putstr_fd("seg_to_args: args NULL\n", 2);
            return NULL;
        }
        seg = seg->next;
    }
    return args;
}

char **word_tokens_to_args(t_token **tk, t_all *all)
{
    char **args;
    char **new_args;

    args = NULL;
    while ((*tk) && (*tk)->type == WORD)
    {
        new_args = seg_to_args((*tk)->seg_list, all);
        if (!new_args || !ft_arrlen(new_args))
        {
            ft_putstr_fd("word_tokens_to_args: !new_str\n", STDERR_FILENO);
            return NULL;
        }
        args = ft_arrconcat_free(args, new_args);
        if (!args || !ft_arrlen((args)))
        {
            ft_putstr_fd("word_tokens_to_args: !args\n", STDERR_FILENO);
            return NULL;
        }
        if (!(*tk)->next || ((*tk)->next && (*tk)->next->type != WORD))
            break;
        (*tk) = (*tk)->next;
    }
    // printf("args -> \n");
    // print_arr(args);
    return args;
}
