#include <parser.h>
#include <built-ins.h>

// segments of token 1. user, exp=false, 2. $VAR, exp=true -> only expand seg 2
char *expand_var(const char *str, t_all *all)
{
    char *var;
    char *res;

    if (ft_strncmp(str, "$?", 3) == 0)
        return ft_itoa(all->process_info->exit_status);
    var = ft_strchr(str, '$');
    var++;
    if (!all->my_env || !all->my_env->envp)
    {
        ft_putstr_fd("parser: expand_var\n", STDERR_FILENO);
        return (NULL);
    }
    res = env_value_dup(var, all->my_env->envp);
    if (!res || (res && !*res))
        return "";
    return res;
}

static char *write_line(char *line, int fd)
{
    ft_putstr_fd(line, fd);
    write(fd, "\n", 1);
    return line;
}

static int expand_each(t_all *all, int fd, char **line)
{
    char *head;
    size_t len;
    char *var;

    head = ft_strchr(*line, '$');
    len = 0;
    if (head)
        len = varname_len(head);
    head++;
    if (!len)
    {
        write_line(*line, fd);
        return 0;
    }
    var = safe_malloc(sizeof(char) * (len + 1), "expand_each");
    if (!var)
        return 1;
    write(fd, *line, --head - *line);
    ft_strlcpy(var, head, len + 1);
    ft_putstr_fd(expand_var(var, all), fd);
    free(var);
    *line = head + len;
    return 0;
}

// expands redir arg user$VAR'more' -> userJOE'more'
// MULTIPLE VARS users$VAR1$VAR2'more' -> userJoeHobs'more'
char *expand_redir_var(t_redir *redir, t_all *all, int fd, char *line)
{
    char *new_line;
    int res;

    if (redir->has_quote || !ft_strchr(line, '$'))
        return write_line(line, fd);
    new_line = line;
    while (new_line)
    {
        res = expand_each(all, fd, &new_line);
        if (res)
            return NULL;
        if (!new_line)
            break;
        if (!ft_strchr(new_line, '$'))
            return write_line(new_line, fd);
    }
    return line;
}
