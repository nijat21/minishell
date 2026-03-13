#include "includes/parser.h"

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

void print_heredoc_eof_warning(const char *str)
{
    ft_putstr_fd("minishell: warning: here-document delimited by end-of-file (wanted `", STDERR_FILENO);
    ft_putstr_fd((char *)str, STDERR_FILENO);
    ft_putstr_fd("')\n", STDERR_FILENO);
}

char *status_to_str(int status)
{
    if (status == 0)
        return "EXIT_SUCCESS";
    else if (status == 1)
        return "EXIT_FAILURE";
    else if (status == 2)
        return "EXIT_MISUSE";
    else if (status == 130)
        return "EXIT_SIGINT";
    return "OTHER";
}

int heredoc_status(bool debug, char *str, int exit_status)
{
    if (debug)
    {
        ft_putstr_fd("minishell: heredoc: `", STDERR_FILENO);
        ft_putstr_fd(str, STDERR_FILENO);
        ft_putstr_fd("' ", STDERR_FILENO);
        ft_putstr_fd("status: `", STDERR_FILENO);
        ft_putstr_fd(status_to_str(exit_status), STDERR_FILENO);
        ft_putstr_fd("'\n", STDERR_FILENO);
    }
    return exit_status;
}