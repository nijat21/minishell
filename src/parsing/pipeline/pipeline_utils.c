#include <parser.h>
#include <lexer.h>

t_redir_type ttype_to_redir_type(t_ttype type)
{
    t_redir_type res;

    if (type == REDIR_OUT)
        res = REDIR_OUTPUT;
    else if (type == REDIR_IN)
        res = REDIR_INPUT;
    else if (type == APPEND)
        res = REDIR_APPEND;
    else if (type == HEREDOC)
        res = REDIR_HEREDOC;
    else
    {
        ft_putstr_fd("Parser: ttype_to_redir_type", STDERR_FILENO);
        return -1;
    }
    return res;
}

size_t count_word_tokens(t_token *tk)
{
    size_t count;
    t_token *temp;

    temp = tk;
    count = 0;
    while (temp && temp->type == WORD)
    {
        count++;
        temp = temp->next;
    }
    return count;
}

static size_t arr_len(char **arr)
{
    size_t i;

    i = 0;
    while (arr && arr[i] && arr[++i])
        i++;
    return i;
}

void free_arr(char **arr)
{
    size_t i;

    i = -1;
    while (arr[++i])
        free(arr[i]);
    free(arr);
}

char **join_args(char **args, char **new_args)
{
    size_t len;
    char **start;
    char **temp;
    size_t i;

    len = arr_len(args) + arr_len(new_args) + 1;
    start = safe_malloc(sizeof(char *) * len);
    if (!start)
        return NULL;
    temp = start;
    i = -1;
    while (args[++i])
        *temp++ = args[i];
    i = -1;
    while (new_args[++i])
        *temp++ = new_args[i];
    *temp = NULL;
    free(args);
    free(new_args);
    return start;
}
