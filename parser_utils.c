#include "parser.h"

void print_unclosed_quote()
{
    ft_putstr_fd("minishell: syntax error: ", STDERR_FILENO);
    ft_putstr_fd("unclosed quote", STDERR_FILENO);
    ft_putstr_fd("\n", STDERR_FILENO);
}

void print_syntax_error(const char *str)
{
    ft_putstr_fd("minishell: syntax error near unexpected token `", STDERR_FILENO);
    ft_putstr_fd((char *)str, STDERR_FILENO);
    ft_putstr_fd("'\n", STDERR_FILENO);
}
