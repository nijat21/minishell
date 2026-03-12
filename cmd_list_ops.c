#include "includes/parser.h"

t_comand *command_lstnew(char *comand, char **args)
{
    t_comand *new;

    new = malloc(sizeof(t_comand));
    if (!new)
        return (NULL);
    new->comand = comand;
    new->args = args;
    new->redir = NULL;
    new->next = NULL;
    return (new);
}

void command_lstadd_back(t_comand **lst, t_comand *new)
{
    t_comand *temp;

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

void command_lstclear(t_comand **lst)
{
    t_comand *temp;
    t_comand *nexttemp;

    temp = *lst;
    while (temp != NULL)
    {
        nexttemp = temp->next;
        free_arr(temp->args);
        redir_lstclear(&temp->redir);
        free(temp);
        temp = nexttemp;
    }
    *lst = NULL;
}

size_t command_lstsize(t_comand *lst)
{
    size_t count;

    count = 0;
    while (lst)
    {
        count++;
        lst = lst->next;
    }
    return (count);
}
