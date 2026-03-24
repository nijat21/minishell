#include <parser.h>

t_cmd *command_lstnew(char *comand, char **args)
{
    t_cmd *new;

    new = malloc(sizeof(t_cmd));
    if (!new)
        return (NULL);
    new->comand = comand;
    new->args = args;
    new->redir = NULL;
    new->next = NULL;
    return (new);
}

void command_lstadd_back(t_cmd **lst, t_cmd *new)
{
    t_cmd *temp;

    temp = *lst;
    if (!new)
    {
        if (lst && *lst)
            command_lstclear(lst);
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

void command_lstclear(t_cmd **lst)
{
    t_cmd *temp;
    t_cmd *nexttemp;

    temp = *lst;
    while (temp != NULL)
    {
        nexttemp = temp->next;
        ft_free_arr(temp->args);
        redir_lstclear(&temp->redir);
        free(temp);
        temp = nexttemp;
    }
    *lst = NULL;
}
