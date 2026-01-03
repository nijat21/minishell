#include "minishell.h"

int shell_exec_error(char *cmd)
{
    if (cmd)
        perror(cmd);
    if (errno == ENOENT)
        exit(127);
    else if (errno == EACCES || errno == EISDIR || errno == ENOTDIR)
        exit(126);
    exit(EXIT_FAILURE);
}

void error_exit(char *msg, bool stop_program, bool use_perror)
{
    if (use_perror)
        perror(msg);
    else
        ft_putendl_fd(msg, STDERR_FILENO);
    if (stop_program)
        exit(EXIT_FAILURE);
}
