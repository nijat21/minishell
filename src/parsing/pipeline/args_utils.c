#include <parser.h>

char **segval_to_args(char *seg_val)
{
    char **args;

    args = safe_malloc(sizeof(char *) * 2, "seg_expand_split");
    if (!args)
        return NULL;
    args[0] = ft_strdup(seg_val);
    args[1] = NULL;
    return args;
}

char **add_str_to_last_arg(char **args, char *seg_val)
{
    int i;
    char *temp;
    
    if (!args || !ft_arrlen(args))
    {
        ft_free_arr(args);
        return segval_to_args(seg_val);
    }
    i = -1;
    while (args[++i])
    {
        if (args[i + 1] == NULL)
        {
            temp = args[i];
            args[i] = ft_strjoin(temp, seg_val);
            free(temp);
            if (!args[i])
            return NULL;
        }
    }
    return args;
}


char **ft_arrconcat(char **args, char **new_args)
{
    size_t len;
    char **start;
    char **temp;
    ssize_t i;

    if (!args && !new_args)
        return NULL;
    len = ft_arrlen(args) + ft_arrlen(new_args) + 1;
    start = safe_malloc(sizeof(char *) * len, "ft_arrconcat_free");
    if (!start)
        return NULL;
    temp = start;
    i = -1;
    while (args && args[++i])
        *temp++ = ft_strdup(args[i]);
    i = -1;
    while (new_args && new_args[++i])
        *temp++ = ft_strdup(new_args[i]);
    *temp = NULL;
    return start;
}

char **ft_arrconcat_free(char **args, char **new_args)
{
    size_t len;
    char **start;
    char **temp;
    ssize_t i;

    if (!args && !new_args)
        return NULL;
    len = ft_arrlen(args) + ft_arrlen(new_args) + 1;
    start = safe_malloc(sizeof(char *) * len, "ft_arrconcat_free");
    if (!start)
        return NULL;
    temp = start;
    i = -1;
    while (args && args[++i])
        *temp++ = ft_strdup(args[i]);
    i = -1;
    while (new_args && new_args[++i])
        *temp++ = ft_strdup(new_args[i]);
    *temp = NULL;
    ft_free_arr(args);
    ft_free_arr(new_args);
    return start;
}

void print_arr(char **arr)
{
    int i;

    i = -1;
    ft_putstr_fd("[ ", STDOUT_FILENO);
    while (arr && arr[++i])
    {
        ft_putstr_fd(arr[i], STDOUT_FILENO);
        ft_putstr_fd(", ", STDOUT_FILENO);
    }
    ft_putstr_fd("NULL ]\n", STDOUT_FILENO);
}