#include "parser.h"

void print_syntax_error(t_token *tk)
{
    ft_putstr_fd("minishell: syntax error near unexpected token `", STDERR_FILENO);
    ft_putstr_fd(tk->seg_list->val, STDERR_FILENO);
    ft_putstr_fd("'\n", STDERR_FILENO);
}

