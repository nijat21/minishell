#include <parser.h>
#include <lexer.h>

static void write_to_heredoc_file(t_redir *redir, char *line)
{
    char *str;
    size_t len;

    len = ft_strlen(line);
    if (len > 0)
        str = ft_strjoin(line, "\n");
    else
        str = ft_strjoin(line, NULL);
    free(line);
    len = ft_strlen(str);
    if (!str)
        exit(EXIT_FAILURE);
    if (!redir->has_quote && ft_strchr(str, '$'))
        expand_redir_arg(str);
    write(redir->write_fd, str, len);
    free(str);
}

static void setup_del(t_redir *redir, char **del)
{
    if (!redir->redir_arg)
        exit(EXIT_MISUSE);
    (*del) = ft_strjoin(NULL, redir->redir_arg);
    if (!(*del))
        exit(EXIT_FAILURE);
    free(redir->redir_arg);
    redir->redir_arg = NULL;
}

static void handle_heredoc(t_redir *redir)
{
    char *del;
    char *line;

    setup_del(redir, &del);
    while (1)
    {
        line = readline("heredoc> ");
        if (!line)
        {
            print_heredoc_eof_warning(del);
            break;
        }
        if (ft_strncmp(line, del, ft_strlen(del) + 1) == 0)
        {
            free(line);
            break;
        }
        write_to_heredoc_file(redir, line);
    }
}

void run_heredocs(t_cmd *cmd)
{
    t_cmd *cmd_iter;
    t_redir *redir_iter;

    cmd_iter = cmd;
    while (cmd_iter)
    {
        redir_iter = cmd_iter->redir;
        while (redir_iter)
        {
            if (redir_iter->type == REDIR_HEREDOC)
                handle_heredoc(redir_iter);
            redir_iter = redir_iter->next;
        }
        cmd_iter = cmd_iter->next;
    }
}