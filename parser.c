#include "minishell.h"

void *parse_input(int ac, char **av)
{
    t_pipex *px;

    px = (t_pipex *)safe_malloc(sizeof(t_pipex));
    if (!px)
        exit(EXIT_FAILURE);
    px->ncmds = ac - 3;
    px->npipes = px->ncmds - 1;
    px->fd_in = open(av[1], O_RDONLY);
    px->fd_out = open(av[ac - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (px->fd_in < 0 || px->fd_out < 0)
    {
        free_px(px);
        error_exit("File", true, true);
    }
    px->pid_ch = safe_malloc(sizeof(int) * px->ncmds);
    if (!px->pid_ch)
    {
        free_px(px);
        error_exit("Malloc", true, true);
    }
    return px;
}
