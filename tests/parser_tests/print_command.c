#include <parser.h>

const char *redir_type_to_str(t_redir_type type)
{
    if (type == REDIR_INPUT)
        return ("REDIR_INPUT (<)");
    if (type == REDIR_OUTPUT)
        return ("REDIR_OUTPUT (>)");
    if (type == REDIR_APPEND)
        return ("REDIR_APPEND (>>)");
    if (type == REDIR_HEREDOC)
        return ("REDIR_HEREDOC (<<)");
    return ("UNKNOWN");
}

void print_redirections(t_redirection *redir)
{
    int i;

    i = 0;
    while (redir)
    {
        printf("      [%d] type: %s | arg: \"%s\" | quote: %s\n",
               i++,
               redir_type_to_str(redir->type),
               redir->redir_arg ? redir->redir_arg : "(null)",
               redir->has_quote ? "YES" : "NO");
        redir = redir->next;
    }
}

void print_comand(t_comand *cmd)
{
    int i;

    i = 0;
    while (cmd)
    {
        printf("┌─ Command [%d]\n", i++);
        printf("│  cmd  : %s\n", cmd->comand ? cmd->comand : "(null)");
        printf("│  args : ");
        if (!cmd->args)
            printf("(null)\n");
        else
        {
            int j = 0;
            printf("[");
            while (cmd->args[j])
            {
                printf("%s", cmd->args[j]);
                if (cmd->args[j + 1])
                    printf(", ");
                j++;
            }
            printf(", NULL]\n");
        }
        printf("│  redirs:\n");
        if (!cmd->redir)
            printf("│    (none)\n");
        else
            print_redirections(cmd->redir);
        printf("└─────────────────────────\n");
        cmd = cmd->next;
    }
}
