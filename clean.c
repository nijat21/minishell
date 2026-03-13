#include "includes/parser.h"

void close_write_fds(t_comand *cmd)
{
    t_comand *cmd_iter;
    t_redirection *redir_iter;

    cmd_iter = cmd;
    while (cmd_iter)
    {
        redir_iter = cmd_iter->redir;
        while (redir_iter)
        {
            if (redir_iter->type == REDIR_HEREDOC)
                close(redir_iter->write_fd);
            redir_iter = redir_iter->next;
        }
        cmd_iter = cmd_iter->next;
    }
}