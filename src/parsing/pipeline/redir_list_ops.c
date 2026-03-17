#include <parser.h>
#include <lexer.h>

t_redir *redir_lstnew(t_redir_type type, char *arg, bool has_quote)
{
    t_redir *new;

    new = safe_malloc(sizeof(t_redir));
    if (!new)
        return (NULL);
    new->type = type;
    new->redir_arg = arg;
    new->has_quote = has_quote;
    new->read_fd = -1;
    new->write_fd = -1;
    new->next = NULL;
    return (new);
}

void redir_lstadd_back(t_redir **lst, t_redir *new)
{
    t_redir *temp;

    temp = *lst;
    if (!new)
    {
        ft_putstr_fd("Parser: redir_lstadd_back: new is NULL\n", 2);
        return;
    }
    if (*lst == NULL)
    {
        *lst = new;
        new->next = NULL;
    }
    else
    {
        while (temp->next != NULL)
            temp = temp->next;
        temp->next = new;
        new->next = NULL;
    }
}

void redir_lstclear(t_redir **lst)
{
    t_redir *temp;
    t_redir *nexttemp;

    temp = *lst;
    while (temp != NULL)
    {
        nexttemp = temp->next;
        free(temp->redir_arg);
        free(temp);
        temp = nexttemp;
    }
    *lst = NULL;
}

size_t redir_lstsize(t_redir *lst)
{
    size_t count;

    count = 0;
    while (!lst)
    {
        count++;
        lst = lst->next;
    }
    return count;
}
