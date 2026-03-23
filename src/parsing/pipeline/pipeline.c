/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nismayil <nismayil@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 15:23:38 by nismayil          #+#    #+#             */
/*   Updated: 2026/03/23 11:35:24 by nismayil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <parser.h>

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

static char *seg_to_str(t_seg *seg, t_all *all)
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

static t_cmd *word_tokens_to_cmd(t_cmd **cmd, t_token **tk, t_all *all)
{
    char **new_args;
    t_cmd *new;

    new_args = word_tokens_to_args(tk, all);
    if (!new_args)
    {
        command_lstclear(cmd);
        return NULL;
    }
    if (*cmd)
    {
        (*cmd)->args = ft_arrconcat_free((*cmd)->args, new_args);
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

static t_redir *token_to_redir(t_cmd **cmd, t_token **tk, t_all *all)
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
    if (type == REDIR_HEREDOC)
        (*tk)->next->seg_list[0].expand = false;
    (*tk) = (*tk)->next;
    if (!(*tk))
        return NULL;
    arg = seg_to_str((*tk)->seg_list, all);
    if (!arg)
        return NULL;
    new_red = redir_lstnew(type, arg, token_has_quote((*tk)->seg_list));
    if (!new_red)
        return free_arg_cmdlst(cmd, arg);
    redir_lstadd_back(&((*cmd)->redir), new_red);
    return (*cmd)->redir;
}

t_cmd **build_pipeline(t_cmd **cmd, t_token *tk, t_all *all)
{
    t_cmd *curr;

    curr = NULL;
    while (tk)
    {
        if (tk->type == WORD)
        {
            curr = word_tokens_to_cmd(&curr, &tk, all);
            if (!curr)
                return NULL;
        }
        else if (ttype_to_tctx(tk->type) == T_REDIRS)
        {
            if (!token_to_redir(&curr, &tk, all))
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
