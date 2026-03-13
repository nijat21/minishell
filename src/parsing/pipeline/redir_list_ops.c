#include <parser.h>

t_redirection *redir_lstnew(t_redir_type type, char *arg, bool has_quote)
{
    t_redirection *new;

    new = safe_malloc(sizeof(t_redirection));
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

void redir_lstadd_back(t_redirection **lst, t_redirection *new)
{
    t_redirection *temp;

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

void redir_lstclear(t_redirection **lst)
{
    t_redirection *temp;
    t_redirection *nexttemp;

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

size_t redir_lstsize(t_redirection *lst)
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
